#include "vulkan_context.h"
#include <stdexcept>

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

void VulkanContext::Cleanup() {}

void VulkanContext::RecreateSwapchain() {}

std::shared_ptr<CommandBuffer> VulkanContext::CreateCommandBuffer() {
    return std::shared_ptr<CommandBuffer>();
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

void VulkanContext::CreateSurface() {}

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
    };
}

void VulkanContext::CreateDebugMessenger() {}

void VulkanContext::CreateCommandPool() {}

void VulkanContext::CreateDescriptorPool() {}

void VulkanContext::CreateFrameContexts() {}

void VulkanContext::DestroyFrameContexts() {}

void VulkanContext::AdvanceFrame() {}

void VulkanContext::BuildVkFeatures() {}
