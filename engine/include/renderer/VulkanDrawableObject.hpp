//
// Created by crypt on 28/07/17.
//
namespace vlk {
    class VulkanDrawableObject;
}

#ifndef VULKAN_ENGINE_CPP_VULKANDRAWABLEOBJECT_H
#define VULKAN_ENGINE_CPP_VULKANDRAWABLEOBJECT_H

#include "RendererDefinition.hpp"
#include "resource/ResourceModel.hpp"
#include "renderer/VulkanModule.hpp"
#include "renderer/VulkanPipelineModule.hpp"

namespace vlk {
    class VulkanDrawableObject {

    private:
        struct VulkanObjects {
            const vk::CommandBuffer commandBuffer;
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfoList;
            std::vector<texture_object> textures;
            vk::DescriptorSetLayout descLayout;
            vk::Pipeline pipeline;
        } vulkan;

        VulkanModule *vulkanModule;
        //VulkanPipelineModule *pipelineModule;
        GameObject *gameObject;

    private:
        void internalPrepareShaders(
                std::vector<vk::ShaderModule> &vertexes,
                std::vector<vk::ShaderModule> &fragments
        );

        void prepareDescriptors();
        void preparePipeline();
        void prepareResourceBuffers();
        void prepareResourceShaders();
        void prepareResources();

    public:

        VulkanDrawableObject(VulkanModule *vulkanModule, GameObject* gameObject);

        // VulkanDrawableObject(
        // vk::CommandBuffer &primary,
        // ResourceModel *renderModel
        // );
        void prepare(vlk::Camera *camera);

        void update();

        void render();

        void buildDrawCommandBuffer(
                vlk::SwapchainImageResources &swapchainImageResources,
                const vk::Viewport *viewport,
                const vk::Rect2D *scissor,
                const vk::RenderPass &renderPass);

        void  getCommandBuffer() {

        }

    };

}

#endif //VULKAN_ENGINE_CPP_VULKANDRAWABLEOBJECT_H
