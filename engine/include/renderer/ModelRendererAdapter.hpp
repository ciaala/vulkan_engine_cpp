//
// Created by crypt on 28/07/17.
//

#ifndef VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H
#define VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H

#include <resource/ResourceModel.hpp>
#include "RendererDefinition.hpp"

namespace vlk {
    class ModelRendererAdapter {
    public:
        void prepareShaders(
                std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfo,
                std::vector<vk::ShaderModule> &vertexes,
                std::vector<vk::ShaderModule> &fragments);

    public:
        // ModelRendererAdapter(vk::CommandBuffer &primary, ResourceModel *renderModel);
        // ModelRendererAdapter();

        void prepare();

        void update();

        void draw();
    };

}

#endif //VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H
