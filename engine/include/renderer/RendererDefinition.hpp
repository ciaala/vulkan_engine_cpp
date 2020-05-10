//
// Created by crypt on 16/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP
#define VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP

#define VK_USE_PLATFORM_XCB_KHR 1
// #define VULKAN_HPP_DISABLE_ENHANCED_MODE 1
#include <memory>
#include <vulkan/vulkan.hpp>
#include "core/CommonMacro.hpp"

namespace vlk {
struct vktexcube_vs_uniform {
  // Must start with MVP
  float mvp[4][4];
  float position[12 * 3][4];
  float attr[12 * 3][4];
};

typedef struct SwapchainImageResources {
  static uint32_t ID_COUNTER;
  uint32_t id;
  vk::Image image;
  std::shared_ptr<vk::CommandBuffer> cmd;
  // TODO should use the ArrayProxy
  std::vector<vk::CommandBuffer> subCommands;
  vk::CommandBuffer graphics_to_present_cmd;
  vk::ImageView view;
  // vk::Buffer uniform_buffer;
  // vk::DeviceMemory uniform_memory;
  vk::Framebuffer framebuffer;
  vk::DescriptorSet descriptor_set;
  ~SwapchainImageResources() {
    cmd.reset();
  }
  SwapchainImageResources() : isReadyClearCommandBuffer(false), id(ID_COUNTER++){}
  bool isReadyClearCommandBuffer;
} SwapchainImageResources;
struct TextureObject {
  vk::Sampler sampler;

  vk::Image image;
  vk::ImageLayout imageLayout{vk::ImageLayout::eUndefined};

  vk::MemoryAllocateInfo mem_alloc;
  vk::DeviceMemory mem;
  vk::ImageView view;

  int32_t tex_width{0};
  int32_t tex_height{0};
};
}
#endif //VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP
