//
// Created by crypt on 21/07/17.
//

#ifndef VULKAN_ENGINE_CPP_AUDIO_HPP
#define VULKAN_ENGINE_CPP_AUDIO_HPP

#include <AL/al.h>

namespace vlk {
class Audio {
 public:
  Audio(char *data, ALenum alFormat, ALsizei rate, ALsizei size);
  ~Audio();
  ALuint getBuffer();

 private:
  char *data;
  ALenum alFormat;
  ALsizei rate;
  ALuint buffer;
  bool isBufferCreated;
  ALsizei size;
};
}

#endif //VULKAN_ENGINE_CPP_AUDIO_HPP
