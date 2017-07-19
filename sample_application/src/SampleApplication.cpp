//
// Created by crypt on 15/07/17.
//

#include "../include/SampleApplication.hpp"
#include <string>

SampleApplication::SampleApplication() : applicationName("SampleApplication") {
    this->game = new SampleGame();
    this->inputController = new SampleInputController(game);
    this->inputController->setWorldObject(static_cast<SampleObject *>(this->getWorld()->getGameObjects().at(0)));
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
