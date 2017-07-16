//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "../include/Engine.hpp"

//    vlk::Engine::Engine(int argc, char **argv) {
//        this->argc = argc;
//        this->argv = argv;
//    }

std::string vlk::Engine::getVulkanVersion() {
    return std::to_string(VK_HEADER_VERSION);
}

vlk::Engine::Engine(Application *application) {
    this->application = application;
    this->setup();
    this->init();
}

void vlk::Engine::setup() {
    this->renderer = new Renderer();
}

void vlk::Engine::init() {
    if (this->DEBUG_LEVEL) {
        std::cout << "Starting up sample_application: " << this->application->getName() << std::endl;
    }

}

void vlk::Engine::draw() {
    std::cout << "Engine.draw" << std::endl;
}

void vlk::Engine::resize() {    std::cout << "Engine.resize" << std::endl;
}