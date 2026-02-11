#pragma once
#include <vulkan/vulkan.h>
#include <vector>

class GraphicsPipelineBuilder {
public:
    GraphicsPipelineBuilder();

    // Adds a shader stage to the pipeline
    GraphicsPipelineBuilder& AddShaderStage(VkShaderStageFlagBits stage, VkShaderModule module,
                                            const char* entry = "main");

    // Sets the vertex input state
    GraphicsPipelineBuilder& SetVertexInput(const VkVertexInputBindingDescription * bindings,
                                            uint32_t bindingCount,
                                            const VkVertexInputAttributeDescription * attributes,
                                            uint32_t attributeCount);

    // Sets the viewport and scissor
    GraphicsPipelineBuilder& SetViewport(VkExtent2D extent);
    GraphicsPipelineBuilder& setViewport(const VkViewport& viewport, VkRect2D scisor);

    // Sets the color blend attachment state
    void SetColorBlendAttachment(const VkPipelineColorBlendAttachmentState& state);

    // Sets the rasterization state
    void SetRasterizationState(const VkPipelineRasterizationStateCreateInfo& state);

    // Sets the depth stencil state
    void SetDepthStencilState(const VkPipelineDepthStencilStateCreateInfo& state);

    // Sets the pipeline layout
    GraphicsPipelineBuilder& SetPipelineLayout(VkPipelineLayout layout);

    // Sets the render pass and subpass
    GraphicsPipelineBuilder& UseRenderPass(VkRenderPass renderPass, uint32_t subpass);

    // Sets dynamic rendering with specified formats
    GraphicsPipelineBuilder& UseDynamicRendering(VkFormat colorFormat,
                                                 VkFormat depthFormat = VK_FORMAT_UNDEFINED);

    // Builds and returns the graphics pipeline
    VkPipeline Build();

    GraphicsPipelineBuilder& SetInputAssembly(const VkPipelineInputAssemblyStateCreateInfo& state);

    GraphicsPipelineBuilder&
    SetTessellationState(const VkPipelineTessellationStateCreateInfo& state);

private:
    VkDevice m_device;

    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages;

    VkPipelineVertexInputStateCreateInfo m_vertexInputInfo{};
    std::vector<VkVertexInputBindingDescription> m_bindingDescriptions;
    std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

    VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState{};
    VkPipelineViewportStateCreateInfo m_viewportState{};
    VkViewport m_viewport{};
    VkRect2D m_scissor{};

    VkPipelineRasterizationStateCreateInfo m_rasterizationState{};
    VkPipelineMultisampleStateCreateInfo m_multisampleState{};
    VkPipelineColorBlendAttachmentState m_colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo m_colorBlendState{};
    VkPipelineDepthStencilStateCreateInfo m_depthStencilState{};

    bool m_tessellationEnabled = false;
    VkPipelineTessellationStateCreateInfo m_tessellationState{};

    VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
    VkFormat m_colorFormat = VK_FORMAT_UNDEFINED;
    VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;

    bool m_useRenderPass = false;
    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    uint32_t m_subpass = 0;
};