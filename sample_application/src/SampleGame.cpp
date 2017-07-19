//
// Created by crypt on 19/07/17.
//

#include <game/GameWorld.hpp>
#include "../include/SampleGame.hpp"

void SampleGame::quit() {
    this->_quit = true;
}

void SampleGame::triggerPause() {
    this->pause = !pause;
}

SampleGame::SampleGame() : gameWorld{new SampleGameWorld()} {}

vlk::GameWorld *SampleGame::getWorld() {
    return this->gameWorld;
}

