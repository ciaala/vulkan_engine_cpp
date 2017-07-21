//
// Created by crypt on 21/07/17.
//

#ifndef VULKAN_ENGINE_CPP_OGGVORBIS_HPP
#define VULKAN_ENGINE_CPP_OGGVORBIS_HPP

#include "Audio.hpp"

namespace vlk {
    class OggVorbisModule {
    public:
        Audio *load(const char *filename);
    };
}


#endif //VULKAN_ENGINE_CPP_OGGVORBIS_HPP
