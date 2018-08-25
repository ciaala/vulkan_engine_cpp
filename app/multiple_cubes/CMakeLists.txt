cmake_minimum_required(VERSION 3.11)

project(multiple_cubes)
set(CMAKE_CXX_STANDARD 17)
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
        src/SampleGameWorld.cpp ../single_cube/src/main.cpp)
include_directories(../../engine/include)

add_library(multiple_cubes_library STATIC ${SOURCE_FILES})
target_link_libraries(multiple_cubes_library vkengine)
target_include_directories(multiple_cubes_library
        PUBLIC vkengine
        PRIVATE include)


set(source "../../resources")
set(destination "${CMAKE_CURRENT_BINARY_DIR}/resources")


add_executable(multiple_cubes src/main.cpp)
target_link_libraries(multiple_cubes vkengine multiple_cubes_library)
# target_include_directories(multiple_cubes PUBLIC
#         vkengine
#        app/multiple_cubes)
#
#add_custom_command(
#        TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E create_symlink ${source} ${destination}
#        DEPENDS ${destination}
#        COMMENT "symbolic link resources folder from ${source} => ${destination}"
#)
