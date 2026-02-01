#include "triangle_app.h"
#include "core/asset_path.h"
#include "core/vulkan_context.h"
#include "core/swapchain.h"
#include "core/shader_loader.h"
#include "core/graphics_pipeline_builder.h"
#include <array>
#include <thread>
#include <stdexcept>

void TriangleApp::OnInitialize()
{
    SetAssetRootPath("../../assets");
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

    vkCmdBindPipeline(*commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
    auto vb = m_vertexBuffer->GetVkBuffer();
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(*commandBuffer, 0, 1, &vb, offsets);
    vkCmdDraw(*commandBuffer, 3, 1, 0, 0);

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
    auto& vulkanCtx = VulkanContext::Get();
    auto& swapchain = vulkanCtx.GetSwapchain();

    VkPipelineLayoutCreateInfo layoutInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
    };
    auto result =
        vkCreatePipelineLayout(vulkanCtx.GetVkDevice(), &layoutInfo, nullptr, &m_pipelineLayout);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to create pipeline layout.");
    }

    vulkanCtx.SetDebugObjectName(
        reinterpret_cast<void*>(m_pipelineLayout),
        VK_OBJECT_TYPE_PIPELINE_LAYOUT, "MyPipelineLayout");

    VkShaderModule vertShaderModule =
        loader::LoadShaderModule(GetAssetPath(AssetType::Shader, "triangle.vert.spv"));
    VkShaderModule fragShaderModule =
        loader::LoadShaderModule(GetAssetPath(AssetType::Shader, "triangle.frag.spv"));

    VkVertexInputBindingDescription bindingDescription{
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };

    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{
        VkVertexInputAttributeDescription{
            .location = 0,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, position),
        },
        VkVertexInputAttributeDescription{
            .location = 1,
            .binding = 0,
            .format = VK_FORMAT_R32G32B32_SFLOAT,
            .offset = offsetof(Vertex, color),
        },
    };

    GraphicsPipelineBuilder builder{};
    builder.AddShaderStage(VK_SHADER_STAGE_VERTEX_BIT, vertShaderModule);
    builder.AddShaderStage(VK_SHADER_STAGE_FRAGMENT_BIT, fragShaderModule);
    builder.SetVertexInput(&bindingDescription, 1,
                           attributeDescriptions.data(),
                           static_cast<uint32_t>(attributeDescriptions.size()));
    auto swapchainExtent = swapchain->GetExtent();
    VkRect2D scissor{
        .offset = {0, 0},
        .extent = swapchainExtent,
    };
    VkViewport viewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(swapchainExtent.width),
        .height = static_cast<float>(swapchainExtent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };
    builder.setViewport(viewport, scissor);
    builder.SetPipelineLayout(m_pipelineLayout);

    auto colorFormat = swapchain->GetFormat().format;
    builder.UseDynamicRendering(colorFormat);
    m_pipeline = builder.Build();

    auto device = vulkanCtx.GetVkDevice();
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
}
