//
// Created by crypt on 19/07/17.
//

#include "../include/SampleGame.hpp"

void SampleGame::quit() {
    this->_quit = true;
}

void SampleGame::triggerPause() {
    this->pause = !pause;
}

SampleCube *SampleGame::getCube() {
    return this->cube;
}

SampleGame::SampleGame() : cube(new SampleCube()) {}

