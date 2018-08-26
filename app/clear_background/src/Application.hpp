//
// Created by crypt on 25/08/18.
//

#ifndef VULKAN_ENGINE_CPP_APPLICATION_HPP
#define VULKAN_ENGINE_CPP_APPLICATION_HPP

#include "game/Application.hpp"

class Application : public vlk::Application {
 private:
  vlk::Engine *engine;
  vlk::GameWorld *world;
  vlk::InputController *inputController;
 public:
  explicit Application(vlk::Engine *engine);
  std::string getName() override;
  vlk::InputController *getInputController() override;
  vlk::GameWorld *getWorld() override;
};

#endif //VULKAN_ENGINE_CPP_APPLICATION_HPP
