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
    this->setupModules();
}

void vlk::Engine::setupModules() {
    this->vulkanModule = new VulkanModule(this, false);
    this->xcbModule = new XCBModule(this);

    this->renderer = new Renderer(this, vulkanModule, xcbModule);
}

void vlk::Engine::init() {
    if (this->DEBUG_LEVEL) {
        std::cout << "Starting up sample_application: " << this->application->getName() << std::endl;
    }
    this->renderer->initWindowLibrary();
    this->renderer->initVulkan();
    this->renderer->createWindow();
    this->renderer->initSwapChain();
    this->prepare();
    this->renderer->prepare(this->application->getBufferData(), this->application->getUVBufferData());
    this->xcbModule->runXCB();
}

void vlk::Engine::draw() {
    static int c = 0;
    static int gc = 0;
    if (c == 1000000) {
        gc ++;
        std::cout << "Engine.draw " << gc << " x 10^6" << std::endl;
        c = 0;
    } else {
        c ++;
    }
    this->vulkanModule->draw();
}

void vlk::Engine::resize() {
    std::cout << "Engine.resize" << std::endl;
}

std::string vlk::Engine::getName() {
    return "Engine";
}

void vlk::Engine::prepare() {
    std::cout << "Engine.prepare" << std::endl;

}

void vlk::Engine::cleanup() {
    std::cout << "Engine.cleanup" << std::endl;

}
