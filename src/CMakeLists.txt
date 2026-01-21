cmake_minimum_required (VERSION 3.19)
project(Vulkan)

set(TARGET Vulkan)

find_package(Vulkan REQUIRED)

set(HDRS
    core/command_buffer.h
    core/glfw_surface_provider.h
    core/swapchain.h
    core/surface_provider.h
    core/vulkan_context.h
    ISampleApp.h
    triangle_app.h
)

set(SRCS
    core/glfw_surface_provider.cpp
    core/vulkan_context.cpp
    core/swapchain.cpp
    main.cpp
)

add_executable(${TARGET} ${HDRS} ${SRCS})

# C++20‚ðŽw’è
set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(${TARGET} PUBLIC ./)

target_link_libraries(${TARGET} PRIVATE Vulkan::Vulkan)
