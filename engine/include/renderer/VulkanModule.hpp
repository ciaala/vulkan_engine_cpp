//
// Created by crypt on 16/07/17.
//
namespace vlk {
class VulkanModule;
}

#ifndef VULKAN_ENGINE_CPP_VULKANMODULE_HPP
#define VULKAN_ENGINE_CPP_VULKANMODULE_HPP
// Allow a maximum of two outstanding presentation operations.
#define FRAME_LAG 2

namespace vlk {
class Engine;
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

// Definition used in prepare

namespace vlk {
class VulkanModule {
 private:
  Engine *engine;


  VulkanPipelineModule *pipelineModule;

  CommandPoolModule *graphicPool;

  uint32_t instance_extension_count = 0;
  uint32_t instance_layer_count = 0;
  uint32_t validation_layer_count = 0;
  char const *const *instance_validation_layers = nullptr;

  uint32_t enabled_extension_count;
  uint32_t enabled_layer_count;
  char const *extension_names[64];
  char const *enabled_layers[64];

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
  vk::PhysicalDeviceProperties gpu_props;
  std::unique_ptr<vk::QueueFamilyProperties[]> queue_props;
  vk::PhysicalDeviceMemoryProperties memory_properties;

  uint32_t queue_family_count;

  bool separate_present_queue;

 public:
  VulkanModule(vlk::Engine *engine, bool validate);
  ~VulkanModule();

  void initDevice();

  void initValidation();

  bool validate;

  vk::Bool32 checkLayers(uint32_t check_count,
                         const char *const *const check_names,
                         uint32_t layer_count,
                         vk::LayerProperties *layers);

  void initSwapChain();

  void initSurface(xcb_connection_t *connection,
                   unsigned int xcb_window);

  vk::SurfaceKHR surface;

  void createDevice();

  void prepare();

  void prepareCubeDataBuffers(Camera *camera,
                              GameObject *object);

 private:

  vk::Format format;
  vk::ColorSpaceKHR color_space;
  bool quit;
  uint32_t curFrame;
  uint32_t frame_index;
  vk::Fence fences[FRAME_LAG];

  void prepareSwapchainBuffers();

  void prepareDepth();

  // Data used inside the prepare phase
  // vk::CommandPool cmd_pool;
  vk::CommandPool present_cmd_pool;

  std::shared_ptr<vk::CommandBuffer> mainCommandBuffer;  // Buffer for initialization commands
  vk::DescriptorSetLayout desc_layout;
  vk::RenderPass render_pass;
  vk::Pipeline globalPipeline;

  uint32_t swapchainImageCount;
  vk::SwapchainKHR swapchain;
  std::unique_ptr<SwapchainImageResources[]> swapchain_image_resources;

  std::vector<texture_object> textures;

  texture_object staging_texture;

  void prepareDescriptorPool();

  void prepareDescriptorSet();

  void prepareFramebuffers();

  void prepareRenderPass();

  struct {
    vk::Format format;
    vk::Image image;
    vk::MemoryAllocateInfo mem_alloc;
    vk::DeviceMemory mem;
    vk::ImageView view;
  } depth;

  uint32_t current_buffer;
  vk::DescriptorPool desc_pool;
  bool prepared{false};
  vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};

  uint32_t width;
  uint32_t height;

  void clearBackgroundCommandBuffer(vk::CommandBuffer *buffer,
                                    vk::Framebuffer &frameBuffer
  );
  void flushInitCmd();

  TextureModule *textureModule;

  void buildImageOwnershipCmd(uint32_t const &index);

  //static char const *const tex_files[];
  bool use_staging_buffer;

  ShaderModule *shaderModule;
  MemoryModule *memoryModule;

  // prepare the draw
 private:
  // float spin_angle;
  void updateDataBuffer(Camera *camera, vk::CommandBuffer &commandBuffer, GameObject *Object);

  // float spin_increment;
  bool pause{false};
 public:
  void resize();

  // Game World Refactor
 public:
  // TODO Move to Texture Module
  vk::FormatProperties textureFormatProperties;
  vk::Format const textureFormat = vk::Format::eR8G8B8A8Unorm;

  void draw(GameWorld *world);

  void prepareTextureFormats();

  void prepareTexture(const char *textureFile, const vk::Format &tex_format);

  void prepareTexture(std::string &basic_string);

  ShaderModule *getShaderModule();

  VulkanPipelineModule *getPipelineModule();
  void drawWorld(vlk::GameWorld *gameWorld);
 private:
  void resetFenceAcquireNextImage();
  void presentFrame();
  vk::Result prepareImageToView(const vk::Image &image, uint32_t index);
  void prepareRenderPassAndFramebuffer();
};
}

#endif //VULKAN_ENGINE_CPP_VULKANMODULE_HPP
