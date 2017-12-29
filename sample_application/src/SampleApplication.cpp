//
// Created by crypt on 15/07/17.
//

#include "../include/SampleApplication.hpp"
#include <string>

SampleApplication::SampleApplication(vlk::Engine *engine) :
    applicationName("SampleApplication") {
  this->engine = engine;
  this->engine->setApplication(this);
  this->game = new SampleGame(engine);
  this->inputController = new SampleInputController(engine, game);
  // TODO remove this IF CHECK
  if (this->getWorld()->getGameObjects().size() > 0) {
    this->inputController->setWorldObject(
        static_cast<SampleObject *>(this->getWorld()->getGameObjects().at(0))
    );
  }
}

std::string SampleApplication::getName() {
  return this->applicationName;
}
//--------------------------------------------------------------------------------------
// Mesh and VertexFormat Data
//--------------------------------------------------------------------------------------
// clang-format off

vlk::InputController *SampleApplication::getInputController() {
  return this->inputController;
}

vlk::GameWorld *SampleApplication::getWorld() {
  return this->game->getWorld();
}

