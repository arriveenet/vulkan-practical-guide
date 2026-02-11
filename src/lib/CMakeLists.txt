cmake_minimum_required (VERSION 3.19)
project(Vulkan)

set(TARGET VulkanLib)

set(HDRS
    common/ISampleApp.h
    core/asset_path.h
    core/buffer_resource.h
    core/command_buffer.h
    core/gpu_resource_base.h
    core/glfw_surface_provider.h
    core/graphics_pipeline_builder.h
    core/image_barrier.h
    core/swapchain.h
    core/surface_provider.h
    core/shader_loader.h
    core/vulkan_context.h
)

set(SRCS
    core/asset_path.cpp
    core/buffer_resource.cpp
    core/command_buffer.cpp
    core/glfw_surface_provider.cpp
    core/graphics_pipeline_builder.cpp
    core/image_barrier.cpp
    core/vulkan_context.cpp
    core/swapchain.cpp
    core/shader_loader.cpp
)

add_library(${TARGET} ${HDRS} ${SRCS})

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${TARGET} PUBLIC ./)

target_link_libraries(${TARGET}
    PRIVATE glfw Vulkan::Vulkan
)
