#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "core/vulkan_context.h"
#include "triangle_app.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(1280, 720, "HelloWindow", nullptr, nullptr);

    auto& vulkanCtx = VulkanContext::Get();
    vulkanCtx.GetWindowSystemExtensions = [=](auto &extensionList) {
        uint32_t extCount = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extCount);
        if (extensions > 0) {
                extensionList.insert(extensionList.end(), extensions, extensions + extCount);
            }
        };

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

