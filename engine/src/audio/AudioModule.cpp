//
// Created by crypt on 21/07/17.
//

#include <iostream>
#include <game/GameWorld.hpp>
#include "AudioModule.hpp"
#include "AudioObject.hpp"

void vlk::AudioModule::init() {
  FLOG(INFO);
  this->device = alcOpenDevice(nullptr);
  if (this->device == nullptr) {
    std::cerr << "Unable to open the audio device";
  }
  this->context = alcCreateContext(this->device, NULL);
  alcMakeContextCurrent(context);
}

vlk::AudioModule::AudioModule() {

}

vlk::Audio *vlk::AudioModule::loadAudio(const std::string &resourceIdentifier) {
  if (this->audioHandleCache.count(resourceIdentifier) > 0) {
    return this->audioHandleCache[resourceIdentifier];
  }
  Audio *audio = this->oggVorbisModule->load(resourceIdentifier.c_str());
  this->audioHandleCache[resourceIdentifier] = audio;
  return audio;
}

ALuint generateSource() {
  ALuint source = 0;

  alGenSources((ALuint) 1, &source);

  alSourcef(source, AL_PITCH, 1);
// TODO check for errors
  alSourcef(source, AL_GAIN, 1);
// TODO check for errors
  alSource3f(source, AL_POSITION, 0, 0, 0);
// TODO check for errors
  alSource3f(source, AL_VELOCITY, 0, 0, 0);
// TODO check for errors
  alSourcei(source, AL_LOOPING, AL_FALSE);
// TODO check for errors
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

void vlk::AudioModule::prepare(GameWorld *world) {
  FLOG(INFO);
  for (auto audioObject : world->getAudioObjects()) {
    const std::string &resouceIdentifier = audioObject->getResouceIdentifier();
    Audio *audio = this->loadAudio(resouceIdentifier);
    audioObject->setAudio(audio);
  }
}

void vlk::AudioModule::render(GameWorld *world) {
  for (auto audioObject : world->getAudioObjects()) {
    if (audioObject->getAudio() != nullptr) {
      this->playAudio(audioObject->getAudio());
    }
  }
}
vlk::AudioModule::~AudioModule() {
  FLOG(INFO) << "Cleaning loaded audio objects";
  for (const auto& cacheEntry: this->audioHandleCache) {
    FLOG(INFO) << "Deleting " << cacheEntry.first;
    delete cacheEntry.second;
  }
}
