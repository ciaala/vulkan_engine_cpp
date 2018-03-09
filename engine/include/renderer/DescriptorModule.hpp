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
        void updateDescriptorSet(std::vector<TextureObject> &textures,
                                 vk::Buffer uniformBuffer,
                                 std::vector<vk::DescriptorSetLayout> &descriptorSetLayoutList,
                                 std::vector<vk::DescriptorSet> &outDescriptorSetList);

        void prepareDescriptorSetLayout(std::vector<TextureObject> &textures,
                                        std::vector<vk::DescriptorSetLayout> &outDescriptorSetLayoutList);

    };
}
#endif //VULKAN_ENGINE_CPP_DESCRIPTORMODULE_HPP
