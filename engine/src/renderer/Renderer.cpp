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
  FLOG(INFO);
#if defined(VK_USE_PLATFORM_XCB_KHR)
  this->xcbModule->initXCBLibrary();
#else
  std::cout << "Unable to idenfiy window library. Supported {xcb}" << std::endl;
#endif
}

void vlk::Renderer::createWindow() {
  FLOG(INFO);
#if defined(VK_USE_PLATFORM_XCB_KHR)

  this->xcbModule->createWindow(vlk::Renderer::WINDOW_WIDTH, vlk::Renderer::WINDOW_HEIGHT);
#else
  std::cout << "Unable to idenfiy window library. Supported {xcb}" << std::endl;
#endif
}

void vlk::Renderer::initVulkan() {
  FLOG(INFO);
  this->vulkanModule->init();
}

vlk::XCBModule *vlk::Renderer::getXCBModule() {
  return this->xcbModule;
}

void vlk::Renderer::initSwapChain() {
  FLOG(INFO);
  vulkanModule->initSurface(xcbModule->getConnection(), xcbModule->getWindow());
  vulkanModule->initSwapChain();
}

/**
 * Make the gameWorld ready to be presented.
 *
 * @param gameWorld
 */
void vlk::Renderer::prepare(GameWorld *gameWorld) {
  FLOG(INFO);
  this->vulkanModule->prepare();

  auto camera = gameWorld->getCamera();
  camera->update();

  // TODO TASK-NEXT prepare the primary command buffer
  this->vulkanModule->preparePrimaryCommandBuffer();


  // TODO Use the spacePartitioner to extract viewables
  // std::iterator<std::input_iterator_tag, GameObject*> viewableIterator = spacePartitioner->getViewableObjectsIterator(gameWorld->getGameObjects());

  //TODO Make PipelineCreation in parallel

  // viewableIterator.next
  // this->vulkanModule->prepareDescriptors(shaderStageInfoList);
  for (GameObject *gameObject : gameWorld->getGameObjects()) {

    // TODO TASK-NEXT What does this function prepare
    this->prepareDrawableObject(camera, gameObject);

    // TODO TASK-NEXT Prepare its own subCommandBuffer
    // this->vulkanModule->prepareDescriptors(shaderStageInfoList);

  }
  // TODO TASK-NEXT Complete the primary command buffer
}

void vlk::Renderer::prepareDrawableObject(vlk::Camera *camera, vlk::GameObject *gameObject) {
  FLOG(INFO);
  // TODO CRITICAL Use a Model identifier.
  VulkanDrawableObject *adapter = nullptr;
  if (this->vulkanDrawables.count(gameObject->getSid()) == 0) {
    adapter = new VulkanDrawableObject(this->vulkanModule, gameObject);
    vulkanDrawables.emplace(gameObject->getSid(), adapter);
  } else {
    adapter = vulkanDrawables[gameObject->getSid()];
  }

  adapter->prepare(camera);
}

void vlk::Renderer::draw(vlk::GameWorld *gameWorld) {
  FLOG(INFO);
  this->vulkanModule->draw(gameWorld);
}
