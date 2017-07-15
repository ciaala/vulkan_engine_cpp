cmake_minimum_required(VERSION 3.7)
project(vulkan_engine_cpp)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)
add_executable(vulkan_engine_cpp ${SOURCE_FILES})
target_link_libraries(vulkan_engine_cpp vulkan)
target_include_directories(vulkan_engine_cpp PUBLIC vulkan)