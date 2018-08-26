//
// Created by crypt on 25/08/18.
//

#include "Application.hpp"

class DummyInputController : public vlk::InputController {
 public:
  void keyReleased(vlk::Keys key) override {

  }
};

class DummyWorld : public vlk::GameWorld {
  vlk::Camera camera;
  std::vector<vlk::AudioObject *> audioObjects;
  std::vector<vlk::GameObject *> gameObjects;
 public:
  std::vector<vlk::GameObject *> getGameObjects() override {
    return gameObjects;
  }
  std::vector<vlk::AudioObject *> getAudioObjects() override {
    return audioObjects;
  }
  void updateWorld() override {

  }
  vlk::Camera *getCamera() override {
    return &camera;
  }
};

Application::Application(vlk::Engine *engine) {
  this->engine = engine;
  this->inputController = new DummyInputController();
  this->world = new DummyWorld();
}
std::string Application::getName() {
  return "clear_background";
}
vlk::InputController *Application::getInputController() {
  return this->inputController;
}
vlk::GameWorld *Application::getWorld() {
  return this->world;
}
