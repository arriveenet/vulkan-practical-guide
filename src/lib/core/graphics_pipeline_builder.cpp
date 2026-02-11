#include "graphics_pipeline_builder.h"
#include "core/vulkan_context.h"

GraphicsPipelineBuilder::GraphicsPipelineBuilder()
{
    m_device = VulkanContext::Get().GetVkDevice();

    m_inputAssemblyState = VkPipelineInputAssemblyStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };

    m_rasterizationState = VkPipelineRasterizationStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };

    m_multisampleState = VkPipelineMultisampleStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 0.0f,
        .pSampleMask = nullptr,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };

    const auto colorWriteAll = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                               VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    m_colorBlendAttachment = VkPipelineColorBlendAttachmentState{
        .blendEnable = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
        .colorBlendOp = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .alphaBlendOp = VK_BLEND_OP_ADD,
        .colorWriteMask = colorWriteAll,
    };
    m_colorBlendState = VkPipelineColorBlendStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .logicOpEnable = VK_FALSE,
        .logicOp = VK_LOGIC_OP_COPY,
        .attachmentCount = 1,
        .pAttachments = &m_colorBlendAttachment,
        .blendConstants = { },
    };
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::AddShaderStage(VkShaderStageFlagBits stage,
                                                                 VkShaderModule module,
                                                                 const char* entry)
{
    VkPipelineShaderStageCreateInfo shaderStageInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .stage = stage,
        .module = module,
        .pName = entry,
    };
    m_shaderStages.push_back(shaderStageInfo);

    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetVertexInput(
    const VkVertexInputBindingDescription* bindings, uint32_t bindingCount,
    const VkVertexInputAttributeDescription* attributes, uint32_t attributeCount)
{
    m_bindingDescriptions.resize(bindingCount);
    for (uint32_t i = 0; i < bindingCount; ++i) {
        m_bindingDescriptions[i] = bindings[i];
    }
    m_attributeDescriptions.resize(attributeCount);
    for (uint32_t i = 0; i < attributeCount; ++i) {
        m_attributeDescriptions[i] = attributes[i];
    }
    m_vertexInputInfo = VkPipelineVertexInputStateCreateInfo{
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .vertexBindingDescriptionCount = bindingCount,
        .pVertexBindingDescriptions = m_bindingDescriptions.data(),
        .vertexAttributeDescriptionCount = attributeCount,
        .pVertexAttributeDescriptions = m_attributeDescriptions.data(),
    };

    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetViewport(VkExtent2D extent)
{
    m_viewport = VkViewport{
        .x = 0.0f,
        .y = 0.0f,
        .width = static_cast<float>(extent.width),
        .height = static_cast<float>(extent.height),
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

     // VK_KHR_Maintenance1 Ç…ÇÊÇÈè„â∫îΩì]
    m_viewport.y = float(extent.height);
    m_viewport.height = -float(extent.height);

    m_scissor = VkRect2D{
        .offset = {0, 0},
        .extent = extent,
    };

    m_viewportState = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                    .viewportCount = 1,
                    .pViewports = &m_viewport,
                    .scissorCount = 1,
                    .pScissors = &m_scissor};

    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::setViewport(const VkViewport& viewport,
                                                              VkRect2D scisor)
{
    m_viewport = viewport;
    m_scissor = scisor;
    m_viewportState = {.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                       .viewportCount = 1,
                       .pViewports = &m_viewport,
                       .scissorCount = 1,
                       .pScissors = &m_scissor};

    return *this;
}

void GraphicsPipelineBuilder::SetColorBlendAttachment(
    const VkPipelineColorBlendAttachmentState& state)
{
    m_colorBlendAttachment = state;
}

void GraphicsPipelineBuilder::SetRasterizationState(
    const VkPipelineRasterizationStateCreateInfo& state)
{
    m_rasterizationState = state;
}

void GraphicsPipelineBuilder::SetDepthStencilState(
    const VkPipelineDepthStencilStateCreateInfo& state)
{
    m_depthStencilState = state;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::SetPipelineLayout(VkPipelineLayout layout)
{
    m_pipelineLayout = layout;

    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::UseRenderPass(VkRenderPass renderPass,
                                                                uint32_t subpass)
{
    m_useRenderPass = true;
    m_renderPass = renderPass;
    m_subpass = subpass;

    return *this;
}

GraphicsPipelineBuilder& GraphicsPipelineBuilder::UseDynamicRendering(VkFormat colorFormat,
                                                                      VkFormat depthFormat)
{
    m_useRenderPass = false;
    m_colorFormat = colorFormat;
    m_depthFormat = depthFormat;

    return *this;
}

VkPipeline GraphicsPipelineBuilder::Build()
{
    VkGraphicsPipelineCreateInfo pipelineInfo{
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = static_cast<uint32_t>(m_shaderStages.size()),
        .pStages = m_shaderStages.data(),
        .pVertexInputState = &m_vertexInputInfo,
        .pInputAssemblyState = &m_inputAssemblyState,
        .pViewportState = &m_viewportState,
        .pRasterizationState = &m_rasterizationState,
        .pMultisampleState = &m_multisampleState,
        .pDepthStencilState = &m_depthStencilState,
        .pColorBlendState = &m_colorBlendState,
        .layout = m_pipelineLayout,
    };

    VkPipelineRenderingCreateInfo renderingInfo{};
    if (m_useRenderPass) {
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.subpass = m_subpass;
    }
    else {
        renderingInfo = VkPipelineRenderingCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
            .colorAttachmentCount = 1,
            .pColorAttachmentFormats = &m_colorFormat,
            .depthAttachmentFormat = m_depthFormat,
        };
        pipelineInfo.pNext = &renderingInfo;
        pipelineInfo.renderPass = VK_NULL_HANDLE;
        pipelineInfo.subpass = 0;
    }

    if (m_tessellationEnabled) {
        pipelineInfo.pTessellationState = &m_tessellationState;
    }

    auto& vulkanCtx = VulkanContext::Get();
    VkPipeline pipeline = VK_NULL_HANDLE;
    if (vkCreateGraphicsPipelines(m_device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) !=
        VK_SUCCESS) {
        return VK_NULL_HANDLE;
    }

    return pipeline;
}

GraphicsPipelineBuilder&
GraphicsPipelineBuilder::SetInputAssembly(const VkPipelineInputAssemblyStateCreateInfo& state)
{
    m_inputAssemblyState = state;

    return *this;
}

GraphicsPipelineBuilder&
GraphicsPipelineBuilder::SetTessellationState(const VkPipelineTessellationStateCreateInfo& state)
{
    m_tessellationState = state;
    m_tessellationEnabled = true;

    return *this;
}
