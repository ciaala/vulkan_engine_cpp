//
// Created by crypt on 31/12/17.
//

#include <glog/log_severity.h>
#include "renderer/VulkanModule.hpp"

vlk::CommandPoolModule::CommandPoolModule(const vk::Device &device,
                                          const uint32_t queueIndex)
    : device(device),
      queueIndex(queueIndex),
      commandBufferCounter(0) {
  createPool();
}
void vlk::CommandPoolModule::createPool() {
  auto const cmd_pool_info = vk::CommandPoolCreateInfo()
      .setQueueFamilyIndex(queueIndex);
  auto result = device
      .createCommandPool(&cmd_pool_info, nullptr, &this->commandPool);
  VERIFY(result == vk::Result::eSuccess);
}
std::shared_ptr<vk::CommandBuffer> vlk::CommandPoolModule::createCommandBuffer() {
  auto result = std::shared_ptr<vk::CommandBuffer>(
      new vk::CommandBuffer,
      [this](vk::CommandBuffer *commandBuffer) {
        uint64_t  id = identifiers[commandBuffer];
        FLOG(INFO) << "Deleting command buffer " << id ;
        device.freeCommandBuffers(commandPool, 1, commandBuffer);
      });
  uint64_t id = commandBufferCounter++;
  identifiers[result.get()] = id;
  FLOG(INFO) << "Creating command buffer " << id;
  auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(this->commandPool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);
  this->device.allocateCommandBuffers(&commandBufferAllocateInfo, result.get());

  return result;
}
std::vector<vk::CommandBuffer> vlk::CommandPoolModule::createCommandBuffers(const uint64_t quantity) {

  FLOG(INFO);
  auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(this->commandPool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(quantity);

  return this->device.allocateCommandBuffers(commandBufferAllocateInfo);
}
vlk::CommandPoolModule::~CommandPoolModule() {
  FLOG(INFO) << "Destroying command pool";
  device.destroyCommandPool(this->commandPool);
}
