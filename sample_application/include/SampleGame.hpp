//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEGAME_HPP
#define VULKAN_ENGINE_CPP_SAMPLEGAME_HPP


#include "SampleCube.hpp"

class SampleGame {
private:
    bool _quit = false;

    bool pause = false;
public:
    SampleGame();

    void quit();

    void triggerPause();

    SampleCube *getCube();

    SampleCube *cube;
};


#endif //VULKAN_ENGINE_CPP_SAMPLEGAME_HPP
