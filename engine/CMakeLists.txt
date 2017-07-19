cmake_minimum_required(VERSION 3.7)
set(CMAKE_MODULE_PATH ../cmake)
project(vkengine)
set(CMAKE_CXX_STANDARD 14)
find_package(XCB REQUIRED)
find_package(Vulkan REQUIRED)

set(SOURCE_FILES src/Engine.cpp
        src/renderer/Renderer.cpp
        src/renderer/VulkanModule.cpp
        src/renderer/XCBModule.cpp
        src/renderer/ShaderModule.cpp
        src/renderer/TextureModule.cpp
        src/renderer/MemoryModule.cpp
        src/core/InputModule.cpp
        )
add_library(vkengine STATIC ${SOURCE_FILES})
target_link_libraries(vkengine ${VULKAN_LIBRARY_DIR} xcb xcb-keysyms)
target_include_directories(vkengine
        PUBLIC ${VULKAN_INCLUDE_DIR}
        PUBLIC xcb
        PRIVATE include)