//
// Created by crypt on 15/07/17.
//
#include <string>
#include "InputController.hpp"

#ifndef VULKAN_ENGINE_CPP_APPLICATION_H
#define VULKAN_ENGINE_CPP_APPLICATION_H

#include "InputController.hpp"
#include "GameObject.hpp"
#include "GameWorld.hpp"


namespace vlk {
    class InputController;

    class GameObject;

    class GameWorld;

    class Application {
    public:

        virtual std::string getName() = 0;

        virtual InputController *getInputController() = 0;

        virtual GameWorld *getWorld() = 0;
    };
}
#endif //VULKAN_ENGINE_CPP_APPLICATION_H
