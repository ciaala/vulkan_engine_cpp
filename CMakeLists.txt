cmake_minimum_required(VERSION 3.8)

project(vulkan_engine_cpp)

set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_CXX_FLAGS -ferror-limit=9001)
set(SOURCE_FILES src/main.cpp)
add_subdirectory(engine)
add_subdirectory(sample_application)
add_subdirectory(snippets)
include_directories(engine/include
        sample_application/include)
add_executable(vulkan_engine_cpp ${SOURCE_FILES})
target_link_libraries(vulkan_engine_cpp vkengine sample_application)
target_include_directories(vulkan_engine_cpp PUBLIC
        vkengine
        sample_application)