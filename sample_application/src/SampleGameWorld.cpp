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

  fillWorld();

  this->audioObjects.emplace_back(new vlk::AudioObject("sample_application/resources/elysium.ogg"));
}
void SampleGameWorld::fillWorld() {

  const auto zeroCube = new SampleObject(engine->getResourceManager()->loadModel("cube0.json"));
  objects.emplace_back(zeroCube);

  const auto firstCube = new SampleObject(engine->getResourceManager()->loadModel("cube1.json"));
  objects.emplace_back(firstCube);
  firstCube->setSpinningAngle(4.0f);
  mat4x4 pos1{0};
  mat4x4_identity(pos1);
  mat4x4_translate(pos1, 1.0, 0.0, 0.0);
  firstCube->setModelMatrix(pos1);

  const auto secondCube = new SampleObject(engine->getResourceManager()->loadModel("cube2.json"));
  objects.emplace_back(secondCube);
  secondCube->setSpinningAngle(-4.0f);
  mat4x4 pos2{0};
  mat4x4_identity(pos2);
  mat4x4_translate(pos2, 1.0, 0.0, 0.0);
  secondCube->setModelMatrix(pos2);
}

std::vector<vlk::AudioObject *> SampleGameWorld::getAudioObjects() {
  return this->audioObjects;
}
SampleGameWorld::~SampleGameWorld() {
  for( auto audioObject : this->audioObjects) {
    delete audioObject;
  }
  audioObjects.clear();
  for(auto object : this->getGameObjects()) {
    delete object;
    }
  this->getGameObjects().clear();
}
