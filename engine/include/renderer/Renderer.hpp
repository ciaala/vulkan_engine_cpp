//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERER_HPP
#define VULKAN_ENGINE_CPP_RENDERER_HPP

#include "VulkanModule.hpp"
#include "XCBModule.hpp"
#include "ShaderModule.hpp"

namespace vlk {
class XCBModule;

class VulkanModule;

class Renderer {

 private:
  static uint16_t const WINDOW_WIDTH;
  static uint16_t const WINDOW_HEIGHT;

#if defined(VK_USE_PLATFORM_XCB_KHR)
  XCBModule *xcbModule;
#endif

  VulkanModule *vulkanModule;
  Engine *engine;

 public:
  XCBModule *getXCBModule();

  Renderer(Engine *engine, VulkanModule *vulkanModule, XCBModule *xcbModule);

  void initWindowLibrary();

  void initVulkan();

  void createWindow();

  void initSwapChain();

  void prepare(GameWorld *gameWorld);

  void draw(GameWorld *gameWorld);

  void
  prepareShaders(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfo,
                 std::vector<vk::ShaderModule> &vertexes,
                 std::vector<vk::ShaderModule> &fragments);
  void prepareGameObject(ShaderModule *shaderModule,
                         Camera *camera,
                         std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
                         GameObject *gameObject);
};
}

#endif //VULKAN_ENGINE_CPP_RENDERER_HPP
