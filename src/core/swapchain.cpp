#include "swapchain.h"

bool Swapchain::Recreate(uint32_t newWidth, uint32_t newHeight)
{
    return false;
}

VkResult Swapchain::QueuePresent(VkQueue queuePresent)
{
    return VkResult();
}

VkSemaphore Swapchain::GetPresentCompleteSemaphore() const
{
    return VkSemaphore();
}

VkSemaphore Swapchain::GetRenderCompleteSemaphore() const
{
    return VkSemaphore();
}

void Swapchain::CreateFrameContext()
{
}

void Swapchain::DestroyFrameContext()
{
}
