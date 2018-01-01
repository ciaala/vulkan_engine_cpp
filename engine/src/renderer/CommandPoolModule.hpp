//
// Created by crypt on 31/12/17.
//

#ifndef VULKAN_ENGINE_CPP_COMMANDBUFFERMODULE_HPP
#define VULKAN_ENGINE_CPP_COMMANDBUFFERMODULE_HPP

#include "renderer/RendererDefinition.hpp"
namespace vlk {
/**
 * Wraps a CommandPool to manage the construction and destruction
 * of CommandBuffers based on the Pool
 */
class CommandPoolModule {
  const vk::Device &device;
  void createPool();
  const uint32_t queueIndex;
  vk::CommandPool commandPool;
  std::unordered_map<vk::CommandBuffer*, uint64_t > identifiers;

 public:
  CommandPoolModule(const vk::Device &device,
                    const uint32_t queueIndex);
  std::shared_ptr<vk::CommandBuffer> createCommandBuffer();
  std::vector<vk::CommandBuffer> createCommandBuffers(uint64_t quantity);
  uint64_t commandBufferCounter;
  ~CommandPoolModule();
  void releaseCommandBuffer(vk::CommandBuffer *pBuffer);
};
}

#endif //VULKAN_ENGINE_CPP_COMMANDBUFFERMODULE_HPP
