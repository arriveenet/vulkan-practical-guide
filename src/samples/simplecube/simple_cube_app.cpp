#include "simple_cube_app.h"
#include "core/asset_path.h"
#include "core/swapchain.h"

void SimpleCubeApp::OnInitialize()
{
    auto assetPath = FindAssetRootPath();
    if (!assetPath.empty()) {
        SetAssetRootPath(assetPath);
    }

    CreateDepthBuffer();
    CreateCubeGeometry();
    CreateDescriptorSetLayout();

    CreateUniformBuffers();
    CreateDescriptorSets();

    CreateGraphicsPipeline();
}

void SimpleCubeApp::OnDrawFrame()
{
    // Drawing code for each frame
}

void SimpleCubeApp::OnCleanup()
{
    // Cleanup code for the simple cube application
}

void SimpleCubeApp::InitializeTriangleVertexBuffer()
{
    // Code to initialize the vertex buffer for the triangle
}

void SimpleCubeApp::InitializeGraphicsPipeline()
{
    // Code to initialize the graphics pipeline
}

void SimpleCubeApp::CreateDepthBuffer()
{
    auto& VulkanContext = VulkanContext::Get();
    auto& swapchain = VulkanContext.GetSwapchain();
    auto extent = swapchain->GetExtent();
    m_depthBuffer = DepthBuffer::Create(extent, VK_FORMAT_D32_SFLOAT);
}

void SimpleCubeApp::CreateCubeGeometry()
{
}

void SimpleCubeApp::CreateDescriptorSetLayout()
{
}

void SimpleCubeApp::CreateUniformBuffers()
{
}

void SimpleCubeApp::CreateDescriptorSets()
{
}

void SimpleCubeApp::CreateGraphicsPipeline()
{
}
