#include "vulkan_context.h"
#include "swapchain.h"
#include "surface_provider.h"

#include <stdexcept>
#include <sstream>
#include <iostream>
#if defined(WIN32)
#   include <Windows.h>
#endif

// Vulkanの構造体pNextを繋ぐ処理簡略化のためのテンプレート
template <typename T> void BuildVkExtentionChain(T &last) {
  last.pNext = nullptr;
}

template <typename T, typename U, typename... Rest>
void BuildVkExtentionChain(T &current, U &next, Rest &...rest) {
  current.pNext = &next;
  BuildVkExtentionChain(next, rest...);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL
VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::stringstream ss;
    ss << "[validation layer] " << pCallbackData->pMessage << std::endl;
#if defined(WIN32)
    OutputDebugStringA(ss.str().c_str());
#else
    std::cerr << ss;
#endif

    return VK_FALSE;
}
//
//static VKAPI_ATTR VkResult VKAPI_CALL vkCreateFence(VkDevice device,
//                                                    const VkFenceCreateInfo* pCreateInfo,
//                                                    const VkAllocationCallbacks* pAllocator,
//                                                    VkFence* pFence);


VulkanContext &VulkanContext::Get()
{
    static VulkanContext instance;
    return instance;
}

void VulkanContext::Initialize(const char *appName,
                               ISurfaceProvider *surfaceProvider) {
    m_surfaceProvider = surfaceProvider;
    CreateInstance(appName); // インスタンスの作成
    PickPhysicalDevice();    // 物理デバイスの選択
    CreateDebugMessenger(); // デバッグ機能の準備
    CreateLogicalDevice();  // 論理デバイスの作成
    CreateCommandPool();    // コマンドプールの作成
    CreateDescriptorPool(); // ディスクリプタプールの作成
}

void VulkanContext::Cleanup()
{
}

void VulkanContext::RecreateSwapchain()
{
    if (m_swapchain == nullptr) {
        m_swapchain = std::make_unique<Swapchain>();
    }

    if (m_surface == VK_NULL_HANDLE) {
        CreateSurface();
    }

    auto width = m_surfaceProvider->GetFrameBufferWidth();
    auto height = m_surfaceProvider->GetFrameBufferHeight();
    m_swapchain->Recreate(width, height);

    DestroyFrameContexts();
    CreateFrameContexts();
}

std::shared_ptr<CommandBuffer> VulkanContext::CreateCommandBuffer()
{
    return std::shared_ptr<CommandBuffer>();
}

VkDescriptorSet
VulkanContext::AllocateDescriptorSet(VkDescriptorSetLayout layout) {
  return VkDescriptorSet();
}

void VulkanContext::FreeDescriptorSet(VkDescriptorSet descriptorSet) {}

VkResult VulkanContext::AcquireNextImage()
{
    return VkResult();
}

void VulkanContext::SubmitPresent() {}

void VulkanContext::SubmitAndWait(
    std::shared_ptr<CommandBuffer> commandBuffer) {}

VulkanContext::FrameContext* VulkanContext::GetCurrentFrameContext()
{
    return nullptr;
}

uint32_t VulkanContext::FindMemoryType(const VkMemoryRequirements &requirements,
                                       VkMemoryPropertyFlags properties) const {
  return 0;
}

void VulkanContext::SetDebugObjectName(void* objectHandle, VkObjectType type, const char* name)
{
#if _DEBUG || DEBUG
    if (m_pfnSetDebugUtilsObjectNameEXT) {
        VkDebugUtilsObjectNameInfoEXT nameInfo{};
        nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
        nameInfo.objectType = type;
        nameInfo.objectHandle = reinterpret_cast<uint64_t>(objectHandle);
        nameInfo.pObjectName = name;
        m_pfnSetDebugUtilsObjectNameEXT(m_vkDevice, &nameInfo);
    }
#endif
}

void VulkanContext::CreateInstance(const char *appName) {
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "VulkanBookEngine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    std::vector<const char *> extensionList;
    std::vector<const char *> layerList;

    GetWindowSystemExtensions(extensionList);

#if DEBUG || _DEBUG
    // デバッグ拡張を有効化
    extensionList.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    // デバッグレイヤーを有効化
    layerList.push_back("VK_LAYER_KHRONOS_validation");
#endif

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = uint32_t(extensionList.size());
    createInfo.ppEnabledExtensionNames = extensionList.data();
    createInfo.enabledLayerCount = uint32_t(layerList.size());
    createInfo.ppEnabledLayerNames = layerList.data();

    if (vkCreateInstance(&createInfo, nullptr, &m_vkInstance) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }
}

void VulkanContext::CreateSurface() {
    m_surface = m_surfaceProvider->CreateSurface(m_vkInstance);

    // プレゼンテーションサポートの確認
    VkBool32 present = VK_FALSE;
    vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, m_graphicsQueueFamilyIndex, m_surface,
                                         &present);
    if (present == VK_FALSE) {
        throw std::runtime_error("selected physical device does not support presentation!");
    }
}

void VulkanContext::PickPhysicalDevice()
{
    uint32_t count = 0;
    vkEnumeratePhysicalDevices(m_vkInstance, &count, nullptr);
    std::vector<VkPhysicalDevice> devices(count);
    vkEnumeratePhysicalDevices(m_vkInstance, &count, devices.data());
    m_vkPhysicalDevice = devices[0];

    // 情報の取得
    vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice,
                                        &m_memoryProperties);
    vkGetPhysicalDeviceProperties(m_vkPhysicalDevice,
                                  &m_physicalDeviceProperties);
}

void VulkanContext::CreateLogicalDevice() {
  // キューファミリのインデックスを取得
  uint32_t queueCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount,
                                           nullptr);
  std::vector<VkQueueFamilyProperties> queues(queueCount);
  vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount,
                                           queues.data());

  m_graphicsQueueFamilyIndex = ~0u;
  for (uint32_t i = 0; const auto &props : queues) {
    if (props.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      m_graphicsQueueFamilyIndex = i;
      break;
    }
    ++i;
  }

    BuildVkFeatures();
    std::vector<const char*> deviceExtensions = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    };

    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;
    VkDeviceCreateInfo deviceInfo{};
    deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceInfo.queueCreateInfoCount = 1;
    deviceInfo.pQueueCreateInfos = &queueInfo;
    deviceInfo.enabledExtensionCount =
        static_cast<uint32_t>(deviceExtensions.size());
    deviceInfo.ppEnabledExtensionNames = deviceExtensions.data();

    deviceInfo.pNext = &m_physicalDevFeatures;
    deviceInfo.pEnabledFeatures = nullptr; // VkPhysicalDeviceFeaturesは使わない

    auto result = vkCreateDevice(m_vkPhysicalDevice, &deviceInfo, nullptr, &m_vkDevice);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    vkGetDeviceQueue(m_vkDevice, m_graphicsQueueFamilyIndex, 0,
                     &m_graphicsQueue);
}

void VulkanContext::CreateDebugMessenger() {
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                 VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                             VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = VulkanDebugCallback;

    auto vkCreateDebugUtilsMessenger = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
        m_vkInstance, "vkCreateDebugUtilsMessengerEXT");

    if (vkCreateDebugUtilsMessenger 
        && vkCreateDebugUtilsMessenger(m_vkInstance, &createInfo, nullptr, &m_debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
    m_pfnSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(
        m_vkInstance, "vkSetDebugUtilsObjectNameEXT");
}

void VulkanContext::CreateCommandPool() {
    VkCommandPoolCreateInfo commandPoolCI{
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
    };
    commandPoolCI.queueFamilyIndex = m_graphicsQueueFamilyIndex;
    commandPoolCI.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(m_vkDevice, &commandPoolCI, nullptr, &m_commandPool);
}

void VulkanContext::CreateDescriptorPool() {}

void VulkanContext::CreateFrameContexts() {}

void VulkanContext::DestroyFrameContexts() {}

void VulkanContext::AdvanceFrame() {}

void VulkanContext::BuildVkFeatures(){
    // デバイスからサポート範囲の情報を取得した後で、使いたいものを有効化する
    // ここでサポートされていない機能を有効化にすると、デバイス作成時にエラーになる
    BuildVkExtentionChain(m_physicalDevFeatures, m_vulkan11Features,
                          m_vulkan12Features, m_vulkan13Features);
    // サポート情報を取得
    vkGetPhysicalDeviceFeatures2(m_vkPhysicalDevice, &m_physicalDevFeatures);

    // 機能を有効化
    m_vulkan13Features.dynamicRendering = VK_TRUE;
    m_vulkan13Features.synchronization2 = VK_TRUE;
}
