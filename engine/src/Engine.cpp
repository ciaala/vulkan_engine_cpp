//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "../include/Engine.hpp"
namespace vlk {
//    engine::engine(int argc, char **argv) {
//        this->argc = argc;
//        this->argv = argv;
//    }

    std::string Engine::getVulkanVersion() {
        return std::to_string(VK_HEADER_VERSION);
    }

    Engine::Engine(Application *application) {
        this->application = application;
        this->setup();
        this->init();
    }
    void Engine::setup() {
        this->renderer = new Renderer();
    }
    void Engine::init() {
        if (this->DEBUG_LEVEL) {
            std::cout << "Starting up sample_application: " << this->application->getName() << std::endl;
        }

    }
}