//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "../include/Renderer.hpp"
#include "../include/XCBModule.hpp"
vlk::Renderer::Renderer(Engine *engine) {
    this->engine = engine;
    //this->initWindowLibrary();
    //this->initVulkan();
}


const uint16_t vlk::Renderer::WINDOW_WIDTH = 500;

const uint16_t vlk::Renderer::WINDOW_HEIGHT = 500;

void vlk::Renderer::initWindowLibrary() {
    std::cout << "Init Window Library" << std::endl;
#if defined(VK_USE_PLATFORM_XCB_KHR)
    this->xcbModule = new XCBModule(this->engine);
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
    this->vulkanModule = new VulkanModule(this->engine, false);
    this->vulkanModule->init();
}

vlk::XCBModule* vlk::Renderer::getXCBModule() {
    return this->xcbModule;
}