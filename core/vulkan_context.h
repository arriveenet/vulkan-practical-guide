#pragma once
#include <glad/vulkan.h>
#include <vector>
#include <memory>
#include <functional>
#include <stdint.h>
#include <string>
#include <cstring>

#include "core/command_buffer.h"

class Swapchain;
class CommandBuffer;
class ISurfaceProvider;

class VulkanContext {
public:
    static constexpr uint32_t MaxInflightFrame = 2;
    static VulkanContext& Get();

    void Initialize(const char* appName, ISurfaceProvider* surfaceProvider);

    void Cleanup();

    // スワップチェインの生成
    void RecreateSwapchain();

    // 各種Vulkanオブジェクトの取得
    VkInstance GetVkInstance() const { return m_vkInstance; }
    VkDevice GetVkDevice() const { return m_vkDevice; }
    VkPhysicalDevice GetVkPhysicalDevice() const { return m_vkPhysicalDevice; }
    VkDescriptorPool GetVkDescriptorPool() const { return m_descriptorPool; }

    VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
    uint32_t GetGraphicsFamily() const { return m_graphicsQueueFamilyIndex; }
    uint32_t GetPresentFamily() const { return m_presentQueueFamilyIndex; }

    VkCommandPool GetCommandPool() const { return m_commandPool; }
    //VkSurfaceKHR GetSurface() const { return m_surface; }

    // コマンドバッファの生成
    std::shared_ptr<CommandBuffer> CreateCommandBuffer();

    struct FrameContext {
      std::shared_ptr<CommandBuffer> commandBuffer;
      VkFence inFlightFence = VK_NULL_HANDLE;
    };

    std::function<void(std::vector<const char*>&)> GetWindowSystemExtensions;

private:
    VulkanContext() = default;
    ~VulkanContext() = default;

private:
    void CreateInstance(const char *appName);
    void CreateSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateDebugMessenger();
    void CreateCommandPool();
    void CreateDescriptorPool();
    void CreateFrameContexts();
    void DestroyFrameContexts();

    void AdvanceFrame();
    void BuildVkFeatures();

    ISurfaceProvider* m_surfaceProvider{};
    VkInstance m_vkInstance{};

    VkPhysicalDevice m_vkPhysicalDevice{};
    VkDevice m_vkDevice{};
    VkQueue m_graphicsQueue{};
    uint32_t m_graphicsQueueFamilyIndex{};
    uint32_t m_presentQueueFamilyIndex{};
    VkPhysicalDeviceMemoryProperties m_memoryProperties{};
    VkPhysicalDeviceProperties m_physicalDeviceProperties{};

   // VkSurfaceKHR m_surface;
    VkCommandPool m_commandPool{};
    VkDescriptorPool m_descriptorPool{};
    std::vector<FrameContext> m_frameContext;
    std::unique_ptr<Swapchain> m_swapchain{};

};

