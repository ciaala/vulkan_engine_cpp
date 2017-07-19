//
// Created by crypt on 15/07/17.
//
#include <string>
#include "InputController.hpp"

#ifndef VULKAN_ENGINE_CPP_APPLICATION_H
#define VULKAN_ENGINE_CPP_APPLICATION_H

#include "InputController.hpp"
#include "GameObject.hpp"

class InputController;
namespace vlk { class GameObject; };

class Application {
public:
    virtual const float *getUVBufferData() = 0;

    virtual const float *getBufferData() = 0;

    virtual std::string getName() = 0;

    virtual InputController *getInputController() = 0;

    virtual vlk::GameObject *getWorld() = 0;
};

#endif //VULKAN_ENGINE_CPP_APPLICATION_H
