//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_GAMEWORLD_HPP
#define VULKAN_ENGINE_CPP_GAMEWORLD_HPP

#include <vector>
#include "GameObject.hpp"
#include "Camera.hpp"
#include "../../src/audio/AudioObject.hpp"

namespace vlk {
class GameWorld {
 public:
  virtual std::vector<GameObject *> getGameObjects() =0;

  virtual std::vector<AudioObject *> getAudioObjects() = 0;
  virtual void updateWorld() = 0;

  virtual Camera *getCamera() = 0;

};
}

#endif //VULKAN_ENGINE_CPP_GAMEWORLD_HPP
