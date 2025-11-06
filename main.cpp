#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "core/vulkan_context.h"
#include "trinagle_app.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(1280, 720, "HelloWindow", nullptr, nullptr);

    auto& vulkanCtx = VulkanContext::Get();

    TriangleApp theApp{};
    theApp.OnInitialize();

    while (glfwWindowShouldClose(window) == GLFW_FALSE)
    {
        glfwPollEvents();

        theApp.OnDrawFrame();
    }

    theApp.OnCleanup();
    vulkanCtx.Cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

