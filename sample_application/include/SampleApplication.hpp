//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
#define VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP

#include <string>
#include <game/GameWorld.hpp>
#include "game/Application.hpp"
#include "SampleInputController.hpp"


class SampleApplication : public vlk::Application {
private:
    std::string applicationName;
    SampleInputController *inputController;
    SampleGame *game;
public:
    SampleApplication();

    std::string getName() override;

    vlk::InputController *getInputController() override;

    ~SampleApplication() {};

    vlk::GameWorld *getWorld() override;

};


#endif //VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
