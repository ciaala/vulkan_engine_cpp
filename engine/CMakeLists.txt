cmake_minimum_required(VERSION 3.7)
set(CMAKE_MODULE_PATH ../cmake)
project(vkengine)
set(CMAKE_CXX_STANDARD 14)
find_package(XCB REQUIRED)
find_package(Vulkan REQUIRED)

set(SOURCE_FILES src/Engine.cpp
        src/Renderer.cpp include/RendererDefinition.hpp src/renderer/VulkanModule.cpp include/VulkanModule.hpp src/renderer/XCBModule.cpp include/XCBModule.hpp src/renderer/ShaderModule.cpp include/ShaderModule.hpp src/renderer/TextureModule.cpp include/TextureModule.hpp include/CommonMacro.hpp src/renderer/MemoryModule.cpp include/MemoryModule.hpp)
add_library(vkengine STATIC ${SOURCE_FILES})
target_link_libraries(vkengine ${VULKAN_LIBRARY_DIR} xcb xcb-keysyms)
target_include_directories(vkengine PUBLIC ${VULKAN_INCLUDE_DIR} PUBLIC xcb)