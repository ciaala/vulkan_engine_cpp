//
// Created by crypt on 19/01/18.
//

#include "renderer/DescriptorModule.hpp"

vlk::DescriptorModule::DescriptorModule(vk::Device &device) : device(device) {

}

void vlk::DescriptorModule::prepareDescriptorSet(std::vector<vk::DescriptorSetLayout> layouts,
                                                 std::vector<TextureObject> &textures,
                                                 vk::DescriptorSet &descriptorSet,
                                                 vk::Buffer uniformBuffer) {
  FLOG(INFO);


  auto buffer_info = vk::DescriptorBufferInfo()
      .setOffset(0)
      .setRange(sizeof(struct vktexcube_vs_uniform));

  vk::DescriptorImageInfo tex_descs[textures.size()];
  for (uint32_t i = 0; i < textures.size(); i++) {
    tex_descs[i].setSampler(textures[i].sampler);
    tex_descs[i].setImageView(textures[i].view);
    tex_descs[i].setImageLayout(vk::ImageLayout::eGeneral);
  }

  vk::WriteDescriptorSet writes[2];

  writes[0].setDescriptorCount(1);
  writes[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
  writes[0].setPBufferInfo(&buffer_info);

  writes[1].setDstBinding(1);
  writes[1].setDescriptorCount((uint32_t) textures.size());
  writes[1].setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
  writes[1].setPImageInfo(tex_descs);

  auto const alloc_info =
      vk::DescriptorSetAllocateInfo()
          .setDescriptorPool(descriptorPool)
          .setDescriptorSetCount(layouts.size())
          .setPSetLayouts(layouts.data());
    auto result = device.allocateDescriptorSets(&alloc_info, &descriptorSet);
    VERIFY(result == vk::Result::eSuccess);

    buffer_info.setBuffer(uniformBuffer);
    writes[0].setDstSet(descriptorSet);

    writes[1].setDstSet(descriptorSet);
    device.updateDescriptorSets(2, writes, 0, nullptr);

}
