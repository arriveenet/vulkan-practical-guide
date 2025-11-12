cmake_minimum_required (VERSION 3.19)
project(Vulkan)

set(TARGET Vulkan)

add_subdirectory(third-party/glad)
add_subdirectory(third-party/glm)

set(HDRS
    core/command_buffer.h
    core/vulkan_context.h
    ISampleApp.h
    triangle_app.h
)

set(SRCS
    core/vulkan_context.cpp
    main.cpp
)

add_executable(${TARGET} ${HDRS} ${SRCS})

target_link_libraries(${TARGET} PUBLIC glad)
