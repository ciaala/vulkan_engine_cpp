//
// Created by crypt on 04/12/17.
//

#ifndef VULKAN_ENGINE_CPP_AUDIOOBJECT_HPP
#define VULKAN_ENGINE_CPP_AUDIOOBJECT_HPP

#include "string"
#include "Audio.hpp"
namespace vlk {

class AudioObject {
  const std::string identifier;
  vlk::Audio *audio;

 public:
  AudioObject(std::string identifier);
  void setAudio(vlk::Audio *audio);

  vlk::Audio *getAudio();

  const std::string &getResouceIdentifier();

};

}
#endif //VULKAN_ENGINE_CPP_AUDIOOBJECT_HPP
