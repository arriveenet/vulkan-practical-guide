#pragma once
#include "surface_provider.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWSurfaceProvider : public ISurfaceProvider {
public:
    explicit GLFWSurfaceProvider(GLFWwindow* window);
    VkSurfaceKHR CreateSurface(VkInstance instance) override;
    uint32_t GetFrameBufferWidth() const override;
    uint32_t GetFrameBufferHeight() const override;

private:
    GLFWwindow* m_window = nullptr;
};
