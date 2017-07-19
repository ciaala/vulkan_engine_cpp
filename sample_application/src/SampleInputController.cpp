//
// Created by crypt on 19/07/17.
//

#include <iostream>
#include "../include/SampleInputController.hpp"

void SampleInputController::keyReleased(vlk::Keys key) {

    switch (key) {
        case vlk::Keys::ARROW_LEFT: {
            this->object->rotateLeft();
            break;
        }
        case vlk::Keys::ARROW_RIGHT: {
            this->object->rotateRight();
            break;
        }
        case vlk::Keys::SPACE_BAR: {
            this->game->triggerPause();
            break;
        }
        case vlk::Keys::ESCAPE: {
            this->game->quit();
            break;
        }
        default :
            std::cout << "released unknown key: " << vlk::InputModule::keyToString.at(key) << std::endl;
            break;
    }
}

SampleInputController::SampleInputController(SampleGame *game) : game(game) {

}

void SampleInputController::setWorldObject(SampleObject *sampleObject) {
    this->object = sampleObject;
}
