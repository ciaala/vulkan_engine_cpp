cmake_minimum_required(VERSION 3.11)

project(vulkan_engine_cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -rdynamic")

set(CMAKE_CXX_CPPCHECK)
#set(CMAKE_CXX_FLAGS -ferror-limit=9001)
# set(SOURCE_FILES app/multiple_cubes/src/main.cpp app/single_cube/src/Application.cpp app/single_cube/src/Application.hpp)
add_subdirectory(engine)
add_subdirectory(app/multiple_cubes)
add_subdirectory(app/clear_background)
add_subdirectory(app/single_cube)
# add_subdirectory(app/snakes)
add_subdirectory(snippets)
add_subdirectory(doc)
include_directories(engine/include
        app/multiple_cubes/include)

