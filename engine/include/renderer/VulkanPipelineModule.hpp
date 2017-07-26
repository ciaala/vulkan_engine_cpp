//
// Created by crypt on 26/07/17.
//

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
        vk::Pipeline preparePipeline(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
                                     vk::PipelineLayout &__pipeline_layout,
                                     vk::RenderPass __render_pass);

        //vk::Pipeline createSubCommandBufferPipeline();

        vk::PipelineLayout preparePipelineLayout(std::vector<texture_object> &textures,
                                                 vk::DescriptorSetLayout &descLayout);

    };
}

#endif //VULKAN_ENGINE_CPP_VULKANPIPELINEMODULE_H
