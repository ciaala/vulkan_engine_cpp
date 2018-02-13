//
// Created by crypt on 19/01/18.
//

#include <glog/log_severity.h>
#include <renderer/VulkanModule.hpp>

vlk::DescriptorModule::DescriptorModule(vk::Device &device) :
    device(device) {
  this->prepareDescriptorPool();
}

void vlk::DescriptorModule::prepareDescriptorSet(std::vector<vk::DescriptorSetLayout> layouts,
                                                 std::vector<TextureObject> &textures,
                                                 std::vector<vk::DescriptorSet> &descriptorSetList,
                                                 vk::Buffer uniformBuffer) {
  FLOG(INFO);

  auto buffer_info = vk::DescriptorBufferInfo()
      .setOffset(0)
      .setRange(sizeof(struct vktexcube_vs_uniform));

  vk::DescriptorImageInfo imageInfo[textures.size()];
  for (uint32_t i = 0; i < textures.size(); i++) {
    imageInfo[i].setSampler(textures[i].sampler);
    imageInfo[i].setImageView(textures[i].view);
    imageInfo[i].setImageLayout(vk::ImageLayout::eGeneral);
  }

  vk::WriteDescriptorSet writes[2];

  writes[0].setDescriptorCount(1);
  writes[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
  writes[0].setPBufferInfo(&buffer_info);

  writes[1].setDstBinding(1);
  writes[1].setDescriptorCount((uint32_t) textures.size());
  writes[1].setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
  writes[1].setPImageInfo(imageInfo);

  auto const allocateInfo = vk::DescriptorSetAllocateInfo()
      .setDescriptorPool(descriptorPool)
      .setDescriptorSetCount(layouts.size())
      .setPSetLayouts(layouts.data());
  auto result = device.allocateDescriptorSets(&allocateInfo, descriptorSetList.data());
  VERIFY(result == vk::Result::eSuccess);

  buffer_info.setBuffer(uniformBuffer);
  writes[0].setDstSet(descriptorSetList[0]);

  writes[1].setDstSet(descriptorSetList[0]);
  device.updateDescriptorSets(2, writes, 0, nullptr);

}
void vlk::DescriptorModule::prepareDescriptorPool() {
  FLOG(INFO);
  // TODO rework this constants
  const auto swapchainImageCount = 3;
  const auto texture_count = 2;
  vk::DescriptorPoolSize const poolSizes[2] = {
      vk::DescriptorPoolSize()
          .setType(vk::DescriptorType::eUniformBuffer)
          .setDescriptorCount(swapchainImageCount),
      vk::DescriptorPoolSize()
          .setType(vk::DescriptorType::eCombinedImageSampler)
          .setDescriptorCount(swapchainImageCount * texture_count)};

  auto const poolCreateInfo = vk::DescriptorPoolCreateInfo()
      .setMaxSets(swapchainImageCount)
      .setPoolSizeCount(2)
      .setPPoolSizes(poolSizes);

  auto result = device.createDescriptorPool(&poolCreateInfo, nullptr, &descriptorPool);
  VERIFY(result == vk::Result::eSuccess);
}
