//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP
#define VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP


#include <game/InputController.hpp>
#include "SampleGame.hpp"

class SampleInputController : public vlk::InputController {
public:
    SampleInputController(SampleGame *game);

    void setWorldObject(SampleObject *sampleObject);

    virtual void keyReleased(vlk::Keys key) override;

private:
    SampleGame *game;
    SampleObject *object;
};


#endif //VULKAN_ENGINE_CPP_SAMPLEINPUTCONTROLLER_HPP
