cmake_minimum_required(VERSION 3.8)

project(create_window)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES create_window.cpp)

add_executable(create_window ${SOURCE_FILES})
target_link_libraries(create_window xcb)
target_include_directories(create_window PUBLIC xcb)