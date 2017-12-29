cmake_minimum_required(VERSION 3.8)
set(CMAKE_MODULE_PATH ../cmake)
project(vkengine)
set(CMAKE_CXX_STANDARD 14)


find_package(XCB REQUIRED)
find_package(Vulkan REQUIRED)
set(SOURCE_FILES
        src/Engine.cpp
        src/renderer/Renderer.cpp
        src/renderer/VulkanModule.cpp
        src/renderer/XCBModule.cpp
        src/renderer/ShaderModule.cpp
        src/renderer/TextureModule.cpp
        src/renderer/MemoryModule.cpp
        src/core/InputModule.cpp

        include/Engine.hpp
        include/renderer/Renderer.hpp
        include/renderer/VulkanModule.hpp
        include/renderer/XCBModule.hpp
        include/renderer/ShaderModule.hpp
        include/renderer/TextureModule.hpp
        include/renderer/MemoryModule.hpp
        include/renderer/RendererDefinition.hpp

        include/core/InputModule.hpp
        include/core/CommonMacro.hpp
        include/core/linmath.h
        include/game/Application.hpp

        include/game/InputController.hpp
        include/game/GameObject.hpp
        src/game/GameObject.cpp
        include/game/GameWorld.hpp
        include/game/Camera.hpp
        src/audio/AudioModule.cpp
        src/audio/AudioModule.hpp
        src/audio/Audio.cpp
        src/audio/Audio.hpp
        src/audio/OggVorbisModule.cpp
        src/audio/OggVorbisModule.hpp
        src/resource/ResourceManager.cpp
        include/resource/ResourceManager.hpp
        src/resource/ResourceModel.cpp
        include/resource/ResourceModel.hpp
        src/renderer/VulkanPipelineModule.cpp
        include/renderer/VulkanPipelineModule.hpp

        include/renderer/VulkanDrawableObject.hpp
        src/renderer/VulkanDrawableObject.cpp

        src/resource/json.hpp
        include/renderer/SpacePartitioner.cpp
        include/renderer/SpacePartitioner.hpp
        src/audio/AudioObject.cpp
        src/audio/AudioObject.hpp)


add_library(vkengine STATIC ${SOURCE_FILES})
target_link_libraries(vkengine ${VULKAN_LIBRARY_DIR}
        xcb
        xcb-keysyms
        openal
        vorbis
        ogg
        vorbisfile
        glog)
target_include_directories(vkengine
        PRIVATE include
        PUBLIC ${VULKAN_INCLUDE_DIR}
        PUBLIC xcb
        PUBLIC openal
        PUBLIC xcb-keysyms
        PUBLIC vorbis
        PUBLIC ogg
        PUBLIC vorbisfile
        PUBLIC glog)
