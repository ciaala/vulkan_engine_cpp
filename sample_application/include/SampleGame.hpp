//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEGAME_HPP
#define VULKAN_ENGINE_CPP_SAMPLEGAME_HPP


#include "SampleObject.hpp"
#include "SampleGameWorld.hpp"

class SampleGame {
private:
    bool _quit = false;

    bool pause = false;

    SampleGameWorld *gameWorld;

public:
    SampleGame(vlk::Engine *engine);

    void quit();

    void triggerPause();

    vlk::GameWorld *getWorld();

};


#endif //VULKAN_ENGINE_CPP_SAMPLEGAME_HPP
