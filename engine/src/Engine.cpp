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
    this->inputModule = new InputModule(this);
    this->audioModule = new AudioModule();
    this->vulkanModule = new VulkanModule(this, true);
    this->xcbModule = new XCBModule(this);

    this->renderer = new Renderer(this, vulkanModule, xcbModule);
}

void vlk::Engine::init() {
    if (this->DEBUG_LEVEL) {
        std::cout << "Starting up sample_application: " << this->application->getName() << std::endl;
    }
    this->audioModule->init();
    this->renderer->initWindowLibrary();
    this->renderer->initVulkan();
    this->renderer->createWindow();
    this->renderer->initSwapChain();
    this->prepare();

    this->xcbModule->runXCB();
}

void vlk::Engine::draw() {
    static int c = 0;
    static int gc = 0;
    if (c == 1000000) {
        gc++;
        std::cout << "Engine.draw " << gc << " x 10^6" << std::endl;
        c = 0;
    } else {
        c++;
    }
    this->renderer->draw(this->application->getWorld());
    this->application->getWorld()->updateWorld();
}

void vlk::Engine::resize() {
    std::cout << "Engine.resize" << std::endl;
}

std::string vlk::Engine::getName() {
    return "Engine";
}

void vlk::Engine::prepare() {
    std::cout << "Engine.prepare" << std::endl;
    this->renderer->prepare(this->application->getWorld());
    Audio *audio = this->audioModule->loadAudio("sample_application/resources/elysium.ogg");
    this->audioModule->playAudio(audio);
}

void vlk::Engine::cleanup() {
    std::cout << "Engine.cleanup" << std::endl;

}
