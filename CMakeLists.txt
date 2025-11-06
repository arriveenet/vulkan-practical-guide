cmake_minimum_required (VERSION 3.19)
project(Vulkan)

set(TARGET Vulkan)

set(HDRS
    core/vulkan_context.h
    ISampleApp.h
    triangle_app.h
)

set(SRCS
    main.cpp
)

add_executable(${TARGET} ${HEDRS} ${SRCS})

