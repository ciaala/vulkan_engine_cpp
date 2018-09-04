//
// Created by crypt on 25/08/18.
//

#include "Application.hpp"
#include "../../multiple_cubes/include/SampleObject.hpp"

class SingleCubeInputController : public vlk::InputController {
 public:
  void keyReleased(vlk::Keys key) override {

  }
};

class DummyWorld : public vlk::GameWorld {
 private:
  vlk::Camera camera;
  std::vector<vlk::AudioObject *> audioObjects;
  std::vector<vlk::GameObject *> gameObjects;
  vlk::Engine &engine;

 public:


  std::vector<vlk::GameObject *> getGameObjects() override {
    const auto zeroCube = new SampleObject(engine.getResourceManager()->loadModel("cube0.json"));
    gameObjects.emplace_back(zeroCube);
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
  explicit DummyWorld(vlk::Engine &engine) : engine(engine) {
  }
};

Application::Application(vlk::Engine *engine) {
  this->engine = engine;
  this->inputController = new SingleCubeInputController();
  this->world = new DummyWorld(*engine);
}
std::string Application::getName() {
  return "single_cube";
}
vlk::InputController *Application::getInputController() {
  return this->inputController;
}
vlk::GameWorld *Application::getWorld() {
  return this->world;
}
