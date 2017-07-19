//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "renderer/Renderer.hpp"

vlk::Renderer::Renderer(Engine *engine, VulkanModule *vulkanModule, XCBModule *xcbModule) {
    this->engine = engine;
    this->vulkanModule = vulkanModule;
    this->xcbModule = xcbModule;
    //this->initWindowLibrary();
    //this->initVulkan();
}


const uint16_t vlk::Renderer::WINDOW_WIDTH = 500;

const uint16_t vlk::Renderer::WINDOW_HEIGHT = 500;

void vlk::Renderer::initWindowLibrary() {
    std::cout << "Init Window Library" << std::endl;
#if defined(VK_USE_PLATFORM_XCB_KHR)
    this->xcbModule->initXCBLibrary();
#else
    std::cout << "Unable to idenfiy window library. Supported {xcb}" << std::endl;
#endif
}

void vlk::Renderer::createWindow() {
#if defined(VK_USE_PLATFORM_XCB_KHR)

    this->xcbModule->createWindow(vlk::Renderer::WINDOW_WIDTH, vlk::Renderer::WINDOW_HEIGHT);
#else
    std::cout << "Unable to idenfiy window library. Supported {xcb}" << std::endl;
#endif
}

void vlk::Renderer::initVulkan() {
    std::cout << "Init Vulkan" << std::endl;
    this->vulkanModule->init();
}

vlk::XCBModule *vlk::Renderer::getXCBModule() {
    return this->xcbModule;
}

void vlk::Renderer::initSwapChain() {
    vulkanModule->initSurface(xcbModule->getConnection(), xcbModule->getWindow());
    vulkanModule->initSwapChain();
}

void vlk::Renderer::prepare(GameWorld *gameWorld) {
    this->vulkanModule->prepare();

    this->vulkanModule->prepareCamera(gameWorld->getCamera());

    for (auto gameObject :gameWorld->getGameObjects()) {
        this->vulkanModule->prepareCubeDataBuffers(gameWorld->getCamera(), gameObject);
        for (std::string &textureFile : gameObject->getTextureFiles()) {
            this->vulkanModule->prepareTexture(textureFile);
        }

    }

    this->vulkanModule->prepareDescriptors();
}

void vlk::Renderer::draw(vlk::GameWorld *gameWorld) {
    this->vulkanModule->draw(gameWorld);


}
