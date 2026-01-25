#include "triangle_app.h"
#include "core/vulkan_context.h"
#include "core/swapchain.h"
#include <thread>

void TriangleApp::OnInitialize()
{
    InitializeTriangleVertexBuffer();
    InitializeGraphicsPipeline();
}

void TriangleApp::OnDrawFrame()
{
    auto& vulkanCtx = VulkanContext::Get();
    auto& swapchain = vulkanCtx.GetSwapchain();
    auto device = vulkanCtx.GetVkDevice();

    if (vulkanCtx.AcquireNextImage() != VK_SUCCESS) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return;
    }

    auto* frameCtx = vulkanCtx.GetCurrentFrameContext();
    auto& commandBuffer = frameCtx->commandBuffer;
    commandBuffer->Begin();

    VkImageSubresourceRange range{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };

    // 描画前：Undefined -> ColorAttachment
    // VK_ATTACHEMENT_LOAD_OP_CLEARを指定のため、常にUNDEFINED指定遷移で問題なし
    commandBuffer->TransitionLayout(swapchain->GetCurrentImage(), range,
                                    ImageLayoutTransition::FromUndefinedToColorAttachment());

    auto imageView = swapchain->GetCurrentView();
    auto extent = swapchain->GetExtent();

    VkRenderingAttachmentInfo colorAttachement{
        .sType = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO,
        .imageView = imageView,
        .imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .clearValue = VkClearValue{.color = {{0.6f, 0.2f, 0.3f, 1.0f}}},
    };
    VkRenderingInfo renderingInfo{
        .sType = VK_STRUCTURE_TYPE_RENDERING_INFO,
        .renderArea = {{0, 0}, extent},
        .layerCount = 1,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachement,
    };
    vkCmdBeginRendering(*commandBuffer, &renderingInfo);
    vkCmdEndRendering(*commandBuffer);

    // 表示用レイアウト変更
    commandBuffer->TransitionLayout(swapchain->GetCurrentImage(), range,
                                    ImageLayoutTransition::FromColorToPresent());

    commandBuffer->End();

    vulkanCtx.SubmitPresent();
}

void TriangleApp::OnCleanup()
{
    auto& vulkanCtx = VulkanContext::Get();
    auto device = vulkanCtx.GetVkDevice();

    vkDeviceWaitIdle(device);
    if (m_pipeline != VK_NULL_HANDLE) {
        vkDestroyPipeline(device, m_pipeline, nullptr);
        m_pipeline = VK_NULL_HANDLE;
    }
    if (m_pipelineLayout != VK_NULL_HANDLE) {
        vkDestroyPipelineLayout(device, m_pipelineLayout, nullptr);
        m_pipelineLayout = VK_NULL_HANDLE;
    }
    m_vertexBuffer->Creanup();
    m_vertexBuffer.reset();
}

void TriangleApp::InitializeTriangleVertexBuffer()
{
    const std::vector<Vertex> triangleVertices = {
        {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}}, // 下頂点（赤）
        {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},  // 右頂点（緑）
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // 左頂点（青）
    };
    VkDeviceSize bufferSize = sizeof(Vertex) * triangleVertices.size();
    m_vertexBuffer = VertexBuffer::Create(bufferSize, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                                                          VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    void* p = m_vertexBuffer->Map();
    memcpy(p, triangleVertices.data(), static_cast<size_t>(bufferSize));
    m_vertexBuffer->Unmap();
}

void TriangleApp::InitializeGraphicsPipeline()
{
}
