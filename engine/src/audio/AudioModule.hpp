//
// Created by crypt on 21/07/17.
//

#ifndef VULKAN_ENGINE_CPP_AUDIOMODULE_HPP
#define VULKAN_ENGINE_CPP_AUDIOMODULE_HPP

#include <AL/alc.h>
#include <unordered_map>
#include "Audio.hpp"
#include "OggVorbisModule.hpp"
#include "core/CommonMacro.hpp"

namespace vlk {
    class AudioModule {
    public:
        AudioModule();

        void init();

        Audio* loadAudio(const std::string &filename);

        void playAudio(Audio* audio);

        void cleanup();

        void prepare(GameWorld *world);
        void render(GameWorld *world);
    private:
        std::unordered_map<Audio*, Audio*> buffers;
        ALCdevice *device;
        ALCcontext *context;
        OggVorbisModule *oggVorbisModule;
        std::unordered_map<std::string, Audio*> audioHandleCache;
    };

}
#endif //VULKAN_ENGINE_CPP_AUDIOMODULE_HPP
