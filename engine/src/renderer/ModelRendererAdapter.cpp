//
// Created by crypt on 28/07/17.
//

#include <iostream>
#include <game/GameObject.hpp>
#include <renderer/VulkanModule.hpp>

void
vlk::ModelRendererAdapter::internalPrepareShaders(
        std::vector<vk::ShaderModule> &vertexes,
        std::vector<vk::ShaderModule> &fragments) {

    for (auto vertex: vertexes) {
        objectType.shaderStageInfoList.emplace_back(
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eVertex)
                        .setModule(vertex)
                        .setPName("main"));
    }
    for (auto fragment: fragments) {
        objectType.shaderStageInfoList.emplace_back(
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eFragment)
                        .setModule(fragment)
                        .setPName("main")
        );
    }
}

void vlk::ModelRendererAdapter::prepare(GameObject *gameObject) {
    for (std::string &textureFile : gameObject->getTextureFiles()) {
        this->vulkanModule->prepareTexture(textureFile);
    }

    std::vector<vk::ShaderModule> vertexes = vulkanModule->getShaderModule()->prepareShaderFromFiles(
            gameObject->getVertexShaderFiles());
    std::vector<vk::ShaderModule> fragments = vulkanModule->getShaderModule()->prepareShaderFromFiles(
            gameObject->getFragmentShaderFiles());
    std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo;
    this->internalPrepareShaders(vertexes, fragments);
}

vlk::ModelRendererAdapter::ModelRendererAdapter(vlk::VulkanModule *vulkanModule)
        : vulkanModule{vulkanModule} {
}

void vlk::ModelRendererAdapter::update() {

}

void vlk::ModelRendererAdapter::draw() {

}

void vlk::ModelRendererAdapter::buildDrawCommandBuffer(
        vlk::SwapchainImageResources &swapchainImageResources,
        const vk::Viewport *viewport,
        const vk::Rect2D *scissor,
        const vk::RenderPass &renderPass) {
    // TODO Command Buffer should be local

    // TOOD ShaderStageInfoList should be local too

    // Initialize the Subordinate Command Buffer with

    auto const inheritanceInfo = vk::CommandBufferInheritanceInfo(renderPass,
                                                                  0,
                                                                  swapchainImageResources.framebuffer);
    auto const commandInfo = vk::CommandBufferBeginInfo()
            .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse |
                      vk::CommandBufferUsageFlagBits::eRenderPassContinue)
            .setPInheritanceInfo(&inheritanceInfo);
    objectType.commandBuffer.begin(commandInfo);

    // TODO decide about pipelineLayout;
    auto __pipeline_layout = vulkanModule->getPipelineModule()->preparePipelineLayout(objectType.textures,
                                                                                      objectType.descLayout);
    auto pipeline = vulkanModule->getPipelineModule()->preparePipeline(objectType.shaderStageInfoList,
                                                                       __pipeline_layout,
                                                                       renderPass);
    //vk::Pipeline pipeline = createSubCommandBufferPipeline();
    objectType.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline);
    //commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->globalPipeline);
    objectType.commandBuffer.setViewport(0, 1, viewport);
    objectType.commandBuffer.setScissor(0, 1, scissor);

    //auto renderPass = vk::SubPass
    //commandBuffer.beginRenderPass(this->render_pass, vk::SubpassContents::eSecondaryCommandBuffers);

    objectType.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                __pipeline_layout,
                                                0,
                                                1,
                                                &(swapchainImageResources.descriptor_set),
                                                0,
                                                nullptr);

    objectType.commandBuffer.draw(12 * 3, 1, 0, 0);
    objectType.commandBuffer.end();
}
