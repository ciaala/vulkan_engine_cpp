//
// Created by crypt on 19/07/17.
//

#include <iostream>
#include "core/InputModule.hpp"


const std::unordered_map<vlk::Keys, std::string> vlk::InputModule::keyToString{
        {vlk::Keys::ARROW_LEFT,  "vlk::Keys::ARROW_LEFT"},
        {vlk::Keys::ARROW_RIGHT, "vlk::Keys::ARROW_RIGHT"},
        {vlk::Keys::SPACE_BAR,   "vlk::Keys::SPACE_BAR"},
        {vlk::Keys::ESCAPE,      "vlk::Keys::ESCAPE"},
};

void vlk::InputModule::sendKeyReleased(Keys key) {
    std::cout << "released key: " << vlk::InputModule::keyToString.at(key) << std::endl;
    this->engine->getApplication()->getInputController()->keyReleased(key);
}

vlk::InputModule::InputModule(vlk::Engine *engine) : engine(engine) {

}


