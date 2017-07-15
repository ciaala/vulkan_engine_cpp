cmake_minimum_required(VERSION 3.7)

project(vkengine)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES src/Engine.cpp
        src/Renderer.cpp )
add_library(vkengine STATIC ${SOURCE_FILES})
target_link_libraries(vkengine vulkan)
target_include_directories(vkengine PUBLIC vulkan)