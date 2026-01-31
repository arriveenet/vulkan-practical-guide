#include "graphics_pipeline_builder.h"
#include "core/vulkan_context.h"

GraphicsPipelineBuilder::GraphicsPipelineBuilder()
{
    m_device = VulkanContext::Get().GetVkDevice();
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::AddShaderStage(VkShaderStageFlagBits stage,
                                                                 VkShaderModule module,
                                                                 const char* entry)
{
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetVertexInput(
    const VkVertexInputBindingDescription* bindings, uint32_t bindingCount,
    const VkVertexInputAttributeDescription* attributes, uint32_t attributeCount)
{
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetViewport(VkExtent2D extent)
{
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::setViewport(const VkViewport& viewport,
                                                              VkRect2D scisor)
{
    return *this;
}

void GraphicsPipelineBuilder::SetColorBlendAttachment(
    const VkPipelineColorBlendAttachmentState& state)
{
}

void GraphicsPipelineBuilder::SetRasterizationState(
    const VkPipelineRasterizationStateCreateInfo& state)
{
}

void GraphicsPipelineBuilder::SetDepthStencilState(
    const VkPipelineDepthStencilStateCreateInfo& state)
{
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetPipelineLayout(VkPipelineLayout layout)
{
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::UseRenderPass(VkRenderPass renderPass,
                                                                uint32_t subpass)
{
    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::UseDynamicRendering(VkFormat colorFormat,
                                                                      VkFormat depthFormat)
{
    return *this;
}

VkPipeline GraphicsPipelineBuilder::Build()
{
    return VkPipeline();
}

GraphicsPipelineBuilder&
GraphicsPipelineBuilder::SetInputAssembly(const VkPipelineInputAssemblyStateCreateInfo& state)
{
    return *this;
}

GraphicsPipelineBuilder&
GraphicsPipelineBuilder::SetTessellationState(const VkPipelineTessellationStateCreateInfo& state)
{
    return *this;
}
