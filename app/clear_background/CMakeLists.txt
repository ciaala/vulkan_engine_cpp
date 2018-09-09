cmake_minimum_required(VERSION 3.11)

project(clear_background)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -rdynamic")

set(SOURCE_FILES
        src/Application.cpp
        src/Application.hpp
        )
include_directories(../../engine/include)

add_library(clear_background_library STATIC ${SOURCE_FILES})
target_link_libraries(clear_background_library vkengine)
target_include_directories(clear_background_library
        PUBLIC vkengine
        PRIVATE include)


set(source "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")

add_executable(clear_background src/main.cpp)
target_link_libraries(clear_background vkengine clear_background_library)
target_include_directories(clear_background PUBLIC
        vkengine
        src)
#
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
#        DEPENDS ${destination}
#        COMMENT "symbolic link resources folder from ${source} => ${destination}"
#)
