//
// Created by crypt on 16/07/17.
//
namespace vlk {
class VulkanModule;
}

#ifndef VULKAN_ENGINE_CPP_VULKANMODULE_HPP
#define VULKAN_ENGINE_CPP_VULKANMODULE_HPP
// Allow a maximum of two outstanding presentation operations.
#define FRAME_LAG 3

static const char *const VK_EXT_DEBUG_REPORT = "VK_EXT_debug_report";
namespace vlk {
class Engine;
class VulkanDebugger;
};

#include "RendererDefinition.hpp"

#include <cstdint>

#include <vulkan/vk_sdk_platform.h>
#include <xcb/xcb.h>

#include <game/GameObject.hpp>
#include <game/Camera.hpp>
#include "core/linmath.h"

#include "Engine.hpp"
#include "TextureModule.hpp"
#include "core/CommonMacro.hpp"
#include "ShaderModule.hpp"
#include "VulkanPipelineModule.hpp"
#include "../../src/renderer/CommandPoolModule.hpp"
#include "VulkanDrawableObject.hpp"
#include "DescriptorModule.hpp"
#include "../../src/renderer/VulkanDebugger.hpp"

// Definition used in prepare

namespace vlk {
class VulkanModule {

  // PRIVATE FIELD
 private:
  Engine *engine;

  VulkanPipelineModule *pipelineModule;
  DescriptorModule *descriptorModule;

  CommandPoolModule *commandPoolGraphic;
  VulkanDebugger *vulkanDebugger;
  uint32_t instance_extension_count = 0;
  uint32_t instance_layer_count = 0;
  uint32_t validation_layer_count = 0;
  char const *const *instance_validation_layers = nullptr;

  // TODO make a vector instead of a static array of pointer to char
  char const *enabled_layers[64];
  uint32_t enabled_layer_count;

  std::vector<const char *> requiredExtensionNames;

  vk::Instance inst;
  vk::PhysicalDevice gpu;
  vk::Device device;
  vk::Queue graphics_queue;
  vk::Queue present_queue;
  uint32_t graphics_queue_family_index;
  uint32_t present_queue_family_index;

  vk::Semaphore image_acquired_semaphores[FRAME_LAG];
  vk::Semaphore draw_complete_semaphores[FRAME_LAG];
  vk::Semaphore image_ownership_semaphores[FRAME_LAG];
  vk::Fence fences[FRAME_LAG];
  uint32_t swapchainImageCount;
  vk::SwapchainKHR swapchain;
  std::unique_ptr<SwapchainImageResources[]> swapchain_image_resources;

  vk::PhysicalDeviceProperties gpu_props;
  std::unique_ptr<vk::QueueFamilyProperties[]> queue_props;
  vk::PhysicalDeviceMemoryProperties memory_properties;

  uint32_t queue_family_count;

  bool separate_present_queue;
  //bool isReadyClearCommandBuffer;

 public:
  VulkanModule(
      vlk::Engine *engine,
      bool validate);

  ~VulkanModule();

  void initDevice();

  void initValidation();

  bool validate;

  vk::Bool32 checkLayers(
      uint32_t check_count,
      const char *const *const check_names,
      uint32_t layer_count,
      vk::LayerProperties *layers);

  void initSwapChain();

  void initSurface(
      xcb_connection_t *connection,
      unsigned int xcb_window);

  vk::SurfaceKHR surface;

  void createDevice();

  void prepare();
  vk::Format &getFormat();
  vk::Format &getDepthFormat();
 private:

  vk::Format format;

 private:
  vk::ColorSpaceKHR color_space;
  bool quit;

  void prepareSwapchainBuffers();

  void prepareDepth();

  vk::CommandPool present_cmd_pool;

  std::shared_ptr<vk::CommandBuffer> mainCommandBuffer;  // Buffer for initialization commands
  vk::DescriptorSetLayout desc_layout;
  vk::RenderPass renderPass;
  vk::Pipeline globalPipeline;

  void prepareFramebuffers();

  void prepareRenderPass();

  struct {
    vk::Format format;
    vk::Image image;
    vk::MemoryAllocateInfo mem_alloc;
    vk::DeviceMemory mem;
    vk::ImageView view;
  } depth;

  uint32_t curFrame;
  uint32_t swapChainIndex;
  vk::DescriptorPool desc_pool;
  bool prepared{false};
  vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};

  uint32_t width;
  uint32_t height;

  void clearBackgroundCommandBuffer(
      vk::CommandBuffer *buffer,
      vk::Framebuffer &frameBuffer,
      std::vector<vk::CommandBuffer> &subCommandBuffers);

  void flushInitCmd();

  TextureModule *textureModule;

  void buildImageOwnershipCmd(
      vk::CommandBuffer &commandBuffer,
      vk::Image &image);

  //static char const *const tex_files[];
  bool use_staging_buffer;

  ShaderModule *shaderModule;
  MemoryModule *memoryModule;

  // prepare the draw
 private:
  // float spin_angle;
  void updateDrawableObject(
      vk::Framebuffer &framebuffer,
      uint32_t width,
      uint32_t height,
      VulkanDrawableObject *drawableObject,
      Camera *camera);

  std::shared_ptr<vlk::VulkanDrawableObject> prepareRenderableObject(
      vk::CommandBuffer &commandBuffer,
      vlk::GameObject *gameObject);

  // float spin_increment;
  bool pause{false};
 public:
  void resize();

  // Game World Refactor
 public:

  void draw(
      GameWorld *world,
      std::unordered_map<GameObject::SID, VulkanDrawableObject *> map);

  ShaderModule *getShaderModule();

  VulkanPipelineModule *getPipelineModule();

  std::vector<vk::CommandBuffer> drawWorld(
      vlk::GameWorld *gameWorld,
      vk::Framebuffer &frameBuffer);

 private:
  void setDynamicStates(vk::CommandBuffer &commandBuffer);
  void resetFenceAcquireNextImage();

  void presentFrame(std::vector<vk::CommandBuffer> &vector);

  vk::Result prepareImageToView(
      const vk::Image &image,
      uint32_t index);

  void prepareRenderPassAndFramebuffer();
  void prepareViewPortAndScissorCommandBuffer(
      vk::Framebuffer &frameBuffer,
      vk::CommandBuffer &commandBufferSubPass,
      float width,
      float height,
      float minDepth,
      float maxDepth) const;

  // NEW PUBLIC API
 public:
  void makeVertexBuffer(
      vlk::vktexcube_vs_uniform &data,
      vk::Buffer &uniformBuffer);

  void
  prepareTextureObject(
      vk::CommandBuffer *commandBuffer,
      std::string &filename,
      TextureObject &textureObject);

  void initSubModules();

  DescriptorModule *getDescriptorModule() const;

  vk::RenderPass &getRenderPass();
  vk::Device *getDevice();
  CommandPoolModule *getCommandPoolModule();
  uint32_t getCurrentFrame();
};
}

#endif //VULKAN_ENGINE_CPP_VULKANMODULE_HPP
