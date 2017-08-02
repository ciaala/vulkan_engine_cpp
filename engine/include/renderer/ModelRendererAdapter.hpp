//
// Created by crypt on 28/07/17.
//
namespace vlk {
    class ModelRendererAdapter;
}

#ifndef VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H
#define VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H

#include "RendererDefinition.hpp"
#include "resource/ResourceModel.hpp"
#include "renderer/VulkanModule.hpp"
#include "renderer/VulkanPipelineModule.hpp"

namespace vlk {
    class ModelRendererAdapter {

    private:
        struct {
            const vk::CommandBuffer commandBuffer;
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfoList;
            std::vector<texture_object> textures;
            vk::DescriptorSetLayout descLayout;
        } objectType;

        VulkanModule *vulkanModule;

    private:
        void internalPrepareShaders(
                std::vector<vk::ShaderModule> &vertexes,
                std::vector<vk::ShaderModule> &fragments
        );

    public:

        explicit ModelRendererAdapter(VulkanModule *vulkanModule);

        // ModelRendererAdapter(
        // vk::CommandBuffer &primary,
        // ResourceModel *renderModel
        // );
        void prepare(GameObject *gameObject);

        void update();

        void draw();

        void buildDrawCommandBuffer(
                vlk::SwapchainImageResources &swapchainImageResources,
                const vk::Viewport *viewport,
                const vk::Rect2D *scissor,

                const vk::RenderPass &renderPass);


    };

}

#endif //VULKAN_ENGINE_CPP_VULKANCOMMANDBUFFER_H
