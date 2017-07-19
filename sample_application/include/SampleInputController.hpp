//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP
#define VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP


#include <game/InputController.hpp>
#include "SampleGame.hpp"

class SampleInputController : public InputController{
public:
    SampleInputController(SampleGame *game);

    virtual void keyReleased(vlk::Keys key) override;

    SampleGame *game;
};


#endif //VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP
