//
// Created by crypt on 15/07/17.
//

#include "renderer/Renderer.hpp"
#include <iostream>

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

    auto camera = gameWorld->getCamera();
    camera->update();

    // TODO TASK-NEXT prepare the primary command buffer
    this->vulkanModule->preparePrimaryCommandBuffer();

    // this->vulkanModule->prepareDescriptors(shaderStageInfoList);
    for (auto gameObject :gameWorld->getGameObjects()) {

        // TODO TASK-NEXT What does this function prepare
        this->prepareGameObject(camera, gameObject);

        // TODO TASK-NEXT Prepare its own subCommandBuffer
        // this->vulkanModule->prepareDescriptors(shaderStageInfoList);
    }
    // TODO TASK-NEXT Complete the primary command buffer
    this->vulkanModule->prepareDescriptors();
}

void vlk::Renderer::prepareGameObject(vlk::Camera *camera, vlk::GameObject *gameObject) {

    this->vulkanModule->prepareCubeDataBuffers(camera, gameObject);

    // TODO CRITICAL Use a Model identifier.
    ModelRendererAdapter *adapter = nullptr;
    if (this->modelRendererAdapters.count("TODO_CONSTANT_NAME_TO_CHANGE") == 0) {
        adapter = new ModelRendererAdapter(this->vulkanModule);
        modelRendererAdapters.emplace("TODO_CONSTANT_NAME_TO_CHANGE", adapter);
    } else {
        adapter = modelRendererAdapters["TODO_CONSTANT_NAME_TO_CHANGE"];
    }
    adapter->prepare(gameObject);
}

void vlk::Renderer::draw(vlk::GameWorld *gameWorld) {
    this->vulkanModule->draw(gameWorld);
}
