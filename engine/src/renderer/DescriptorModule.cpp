//
// Created by crypt on 19/01/18.
//


#include <renderer/VulkanModule.hpp>

const int TEMP_OBJECTS_COUNT = 10;
vlk::DescriptorModule::DescriptorModule(vk::Device &device) :
        device(device) {
    this->prepareDescriptorPool();
}

/**
 * this function prepares the descriptor pools
 * - uniformBuffer
 * - eCombinedImageSampler
 *
 */
void vlk::DescriptorModule::prepareDescriptorPool() {
    FLOG(INFO);
    LOG(WARNING) << "Rework this constants " << std::endl;

    // TODO rework this constants
    const auto swapchainImageCount = 3;
    const auto texture_count = 2;
    std::vector<vk::DescriptorPoolSize> poolSizes = {
            vk::DescriptorPoolSize()
                    .setType(vk::DescriptorType::eUniformBuffer)
                    .setDescriptorCount(swapchainImageCount* TEMP_OBJECTS_COUNT),
            vk::DescriptorPoolSize()
                    .setType(vk::DescriptorType::eCombinedImageSampler)
                    .setDescriptorCount(swapchainImageCount * texture_count* TEMP_OBJECTS_COUNT),
    };

    auto const poolCreateInfo = vk::DescriptorPoolCreateInfo()
            .setMaxSets(swapchainImageCount)
            .setPoolSizeCount((uint32_t) poolSizes.size())
            .setPPoolSizes(poolSizes.data());

    auto result = device.createDescriptorPool(&poolCreateInfo, nullptr, &descriptorPool);
    VERIFY(result == vk::Result::eSuccess);
}

void vlk::DescriptorModule::updateDescriptorSet(std::vector<TextureObject> &textures,
                                                vk::Buffer uniformBuffer,
                                                std::vector<vk::DescriptorSetLayout> &descriptorSetLayoutList,
                                                std::vector<vk::DescriptorSet> &outDescriptorSetList) {
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
  if ( outDescriptorSetList.empty()) {
    outDescriptorSetList.resize(1);
    auto const allocateInfo = vk::DescriptorSetAllocateInfo()
        .setDescriptorPool(descriptorPool)
        .setDescriptorSetCount((uint32_t) outDescriptorSetList.size())
        .setPSetLayouts(descriptorSetLayoutList.data());
    auto result = device.allocateDescriptorSets(&allocateInfo, outDescriptorSetList.data());
    VERIFY(result == vk::Result::eSuccess);
  }
    buffer_info.setBuffer(uniformBuffer);
    writes[0].setDstSet(outDescriptorSetList[0]);

    writes[1].setDstSet(outDescriptorSetList[0]);
    device.updateDescriptorSets(2, writes, 0, nullptr);

}

void vlk::DescriptorModule::prepareDescriptorSetLayout(std::vector<TextureObject> &textures,
                                                       std::vector<vk::DescriptorSetLayout> &outDescriptorSetLayoutList) {
    FLOG(INFO);
    vk::DescriptorSetLayoutBinding const layoutBinding[2] = {
            vk::DescriptorSetLayoutBinding()
                    .setBinding(0)
                    .setDescriptorType(
                            vk::DescriptorType::eUniformBuffer)
                    .setDescriptorCount(1)
                    .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                    .setPImmutableSamplers(nullptr),
            vk::DescriptorSetLayoutBinding()
                    .setBinding(1)
                    .setDescriptorType(
                            vk::DescriptorType::eCombinedImageSampler)
                    .setDescriptorCount((uint32_t) textures.size())
                    .setStageFlags(
                            vk::ShaderStageFlagBits::eFragment)
                    .setPImmutableSamplers(nullptr)};

    auto const descriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo()
            .setBindingCount(2)
            .setPBindings(layoutBinding);
    outDescriptorSetLayoutList.reserve(1);
    auto result = device.createDescriptorSetLayout(&descriptorSetLayoutCreateInfo,
                                                   nullptr,
                                                   outDescriptorSetLayoutList.data());
    VERIFY(result == vk::Result::eSuccess);

}
