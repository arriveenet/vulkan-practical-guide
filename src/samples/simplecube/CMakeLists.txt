cmake_minimum_required (VERSION 3.19)
project(SimpleCube)

set(TARGET SimpleCube)

set(HDRS
    simple_cube_app.h
)

set(SRCS
    simple_cube_app.cpp
    main.cpp
)

add_executable(${TARGET} ${HDRS} ${SRCS})

set_target_properties(${TARGET} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_link_libraries(${TARGET}
    PRIVATE VulkanLib Vulkan::Vulkan glfw glm
)

