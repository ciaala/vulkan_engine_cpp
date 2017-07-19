cmake_minimum_required(VERSION 3.8)

project(sample_application)
set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES
        src/SampleApplication.cpp
        include/SampleApplication.hpp
        src/SampleInputController.cpp
        include/SampleInputController.hpp
        src/SampleGame.cpp
        include/SampleGame.hpp
        src/SampleObject.cpp
        include/SampleObject.hpp
        include/SampleGameWorld.hpp
        src/SampleGameWorld.cpp)
include_directories(../engine/include)

add_library(sample_application STATIC ${SOURCE_FILES})
target_include_directories(sample_application
        PUBLIC vkengine
        PRIVATE include)


set (source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set (destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "symbolic link resources folder from ${source} => ${destination}"
)
