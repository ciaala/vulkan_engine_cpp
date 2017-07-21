//
// Created by crypt on 21/07/17.
//

#include <iostream>
#include "AudioModule.hpp"


void vlk::AudioModule::init() {
    this->device = alcOpenDevice(nullptr);
    if (this->device == nullptr) {
        std::cerr << "Unable to open the audio device";
    }
    this->context = alcCreateContext(this->device, NULL);
    alcMakeContextCurrent(context);
}


vlk::AudioModule::AudioModule() {

}

vlk::Audio *vlk::AudioModule::loadAudio(const std::string &filename) {
    if (this->audioHandleCache.count(filename) > 0) {
        return this->audioHandleCache[filename];
    }
    Audio *audio = this->oggVorbisModule->load(filename.c_str());
    this->audioHandleCache[filename] = audio;
}

ALuint generateSource() {
    ALuint source = 0;

    alGenSources((ALuint) 1, &source);

    alSourcef(source, AL_PITCH, 1);
// check for errors
    alSourcef(source, AL_GAIN, 1);
// check for errors
    alSource3f(source, AL_POSITION, 0, 0, 0);
// check for errors
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
// check for errors
    alSourcei(source, AL_LOOPING, AL_FALSE);
// check for errors
    return source;
}

void vlk::AudioModule::playAudio(vlk::Audio *audio) {
    ALuint source = generateSource();
    alGenSources((ALuint) 1, &source);
    alSourcei(source, AL_BUFFER, audio->getBuffer());
    alSourcePlay(source);
}

void vlk::AudioModule::cleanup() {
    if (this->device != nullptr) {
        alcCloseDevice(this->device);
    }
}
