//
// Created by crypt on 19/07/17.
//

#include <game/GameObject.hpp>
#include <vector>
#include <iostream>
#include <SampleObject.hpp>
#include <Engine.hpp>
#include "SampleGameWorld.hpp"
#include "resource/ResourceModel.hpp"

std::vector<vlk::GameObject *> SampleGameWorld::getGameObjects() {
  return this->objects;
}

void SampleGameWorld::updateWorld() {
  static bool once = false;
  if (!once) {
    LOG(INFO) << "Update the world has been called multiple times" << std::endl;
  }
  once = true;
}

vlk::Camera *SampleGameWorld::getCamera() {
  return &this->camera;
}

SampleGameWorld::SampleGameWorld(vlk::Engine *engine) {
  this->engine = engine;
  // TODO Re-enable
  // fillWorld();

  this->audioObjects.emplace_back(new vlk::AudioObject("sample_application/resources/elysium.ogg"));
}
void SampleGameWorld::fillWorld() {
  vlk::ResourceModel *resourceModel = engine->getResourceManager()->loadModel("cube.json");
  objects.emplace_back(new SampleObject(resourceModel));

  auto secondCube = new SampleObject(engine->getResourceManager()->loadModel("cube2.json"));
  secondCube->setSpinningAngle(-4.0f);
  mat4x4 pos{0};
  mat4x4_identity(pos);
  mat4x4_translate(pos, 1.0, 0.0, 0.0);
  secondCube->setModelMatrix(pos);
  objects.emplace_back(secondCube);
}

std::vector<vlk::AudioObject *> SampleGameWorld::getAudioObjects() {
  return this->audioObjects;
}
