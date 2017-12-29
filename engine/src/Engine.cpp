//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "Engine.hpp"

//    vlk::Engine::Engine(int argc, char **argv) {
//        this->argc = argc;
//        this->argv = argv;
//    }
void vlk::Engine::static_initialization() {
  google::InitGoogleLogging(vlk::Engine::getName().c_str());

  FLOG(INFO) << "VulkanEngine startup." << std::endl;
  FLOG(INFO) << "Vulkan version: " << vlk::Engine::getVulkanVersion() << std::endl;

}
std::string vlk::Engine::getVulkanVersion() {
  return std::to_string(VK_HEADER_VERSION);
}

vlk::Engine::Engine() {
  this->setupModules();
}

void vlk::Engine::setupModules() {
  this->inputModule = new InputModule(this);
  this->audioModule = new AudioModule();
  this->vulkanModule = new VulkanModule(this, true);
  this->xcbModule = new XCBModule(this);
  this->resourceManager = new ResourceManager("sample_application/resources");
  this->renderer = new Renderer(this, vulkanModule, xcbModule);
}

void vlk::Engine::init() {

  FLAGS_logtostderr = 1;
  FLOG(INFO) << "Starting up application: " << this->application->getName() << std::endl;

  this->audioModule->init();
  this->renderer->init();

  this->prepare();

  this->xcbModule->runXCB();

}

void vlk::Engine::draw() {
  static int c = 0;
  static int gc = 0;
  if (c % 1000 == 0) {

    FLOG(INFO) << "Drawing {frame: " << gc << "000, objects: " << this->application->getWorld()->getGameObjects().size() << "}" << std::endl;
    c = 1;
    gc++;
  } else {
    c++;
  }
  this->renderer->draw(this->application->getWorld());
  this->application->getWorld()->updateWorld();
}

void vlk::Engine::resize(uint32_t width, u_int32_t height) {
  FLOG(INFO) << "INCOMPLETE" << std::endl;
}

std::string vlk::Engine::getName() {
  return "Engine";
}

void vlk::Engine::prepare() {
  FLOG(INFO) << std::endl;
  this->audioModule->prepare(this->application->getWorld());

  this->renderer->prepare(this->application->getWorld());

}

void vlk::Engine::cleanup() {
  FLOG(INFO);
  this->audioModule->cleanup();
}

void vlk::Engine::queue_audio_effect(vlk::GameObject *gameObject, const std::string audioFilename) {
  auto audio = this->audioModule->loadAudio(audioFilename);
  this->audioModule->playAudio(audio);
}

vlk::ResourceManager *vlk::Engine::getResourceManager() {
  return this->resourceManager;
}

bool vlk::Engine::isReady() {
  return true;
}