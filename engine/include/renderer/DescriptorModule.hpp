//
// Created by crypt on 19/01/18.
//

#ifndef VULKAN_ENGINE_CPP_DESCRIPTORMODULE_HPP
#define VULKAN_ENGINE_CPP_DESCRIPTORMODULE_HPP
#include <renderer/RendererDefinition.hpp>

namespace vlk {
class DescriptorModule {

 private:
  vk::Device &device;
  vk::DescriptorPool descriptorPool;
  void prepareDescriptorPool();

 public:
  explicit DescriptorModule(vk::Device &device);

 public:
  void prepareDescriptorSet(std::vector<vk::DescriptorSetLayout> layouts,
                            std::vector<TextureObject> &textures,
                            std::vector<vk::DescriptorSet> &descriptorSetList,
                            vk::Buffer uniformBuffer);

};
}
#endif //VULKAN_ENGINE_CPP_DESCRIPTORMODULE_HPP
