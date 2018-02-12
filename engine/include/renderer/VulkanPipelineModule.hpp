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
        vk::Pipeline prepareGraphicPipeline(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
                                            vk::PipelineLayout &__pipeline_layout,
                                            vk::RenderPass __render_pass);

        //vk::Pipeline createSubCommandBufferPipeline();

        void preparePipelineLayout(std::vector<TextureObject> &textures,
                                           vk::DescriptorSetLayout &descLayout,
                                           vk::PipelineLayout &pipelineLayout);

    };
}

#endif //VULKAN_ENGINE_CPP_VULKANPIPELINEMODULE_H
