cmake_minimum_required(VERSION 3.11)

project(single_cube)
set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES
        src/Application.cpp
        src/Application.hpp
        src/SampleObject.hpp
        src/SampleObject.cpp
        )
include_directories(../../engine/include)

add_library(single_cube_library STATIC ${SOURCE_FILES})
target_link_libraries(single_cube_library vkengine)
target_include_directories(single_cube_library
        PUBLIC vkengine
        PRIVATE include)


set(source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_executable(single_cube src/main.cpp)
target_link_libraries(single_cube vkengine single_cube_library)
target_include_directories(single_cube PUBLIC
        vkengine
        src)
#
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
#        DEPENDS ${destination}
#        COMMENT "symbolic link resources folder from ${source} => ${destination}"
#)
