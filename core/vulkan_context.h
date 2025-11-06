#pragma once
#include <vulkan/vulkan.h>
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

    std::function<void(std::vector<const char*>&)> GetWindowSystemExtensions;

private:
    VulkanContext() = default;
    ~VulkanContext() = default;

    ISurfaceProvider* m_surfaceProvider{};

};

