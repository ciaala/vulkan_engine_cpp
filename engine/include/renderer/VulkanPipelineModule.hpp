//
// Created by crypt on 26/07/17.
//
namespace vlk {
    class VulkanPipelineModule;
}
#ifndef VULKAN_ENGINE_CPP_VULKANPIPELINEMODULE_H
#define VULKAN_ENGINE_CPP_VULKANPIPELINEMODULE_H

#include "RendererDefinition.hpp"

namespace vlk {

    class VulkanPipelineModule {
    private:
        vk::PipelineCache pipelineCache;
        vk::Device *device;
        std::vector<vk::Pipeline> pipelineList;
        std::vector<vk::PipelineLayout> pipelineLayoutList;
    public:
        explicit VulkanPipelineModule(vk::Device *device);

        ~VulkanPipelineModule();

    public:
        void prepareGraphicPipeline(
                std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
                vk::PipelineLayout &pipelineLayout,
                vk::RenderPass &renderPass,
                vk::Pipeline &outPipeline);

        //vk::Pipeline createSubCommandBufferPipeline();

        void preparePipelineLayout(std::vector<vk::DescriptorSetLayout> &descriptorSetLayoutList,
                                   vk::PipelineLayout &outPipelineLayout);


    };
}

#endif //VULKAN_ENGINE_CPP_VULKANPIPELINEMODULE_H
