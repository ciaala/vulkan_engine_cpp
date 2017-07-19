cmake_minimum_required(VERSION 3.7)

project(sample_application)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        src/SampleApplication.cpp
        include/SampleApplication.hpp
        src/SampleInputController.cpp
        include/SampleInputController.hpp src/SampleGame.cpp include/SampleGame.hpp src/SampleCube.cpp include/SampleCube.hpp)
include_directories(../engine/include)

add_library(sample_application STATIC ${SOURCE_FILES})
target_include_directories(sample_application PUBLIC vkengine)