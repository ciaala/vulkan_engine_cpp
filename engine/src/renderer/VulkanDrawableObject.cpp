//
// Created by crypt on 28/07/17.
//

#include <iostream>
#include <game/GameObject.hpp>
#include <renderer/VulkanModule.hpp>

void
vlk::VulkanDrawableObject::internalPrepareShaders(
    std::vector<vk::ShaderModule> &vertexes,
    std::vector<vk::ShaderModule> &fragments) {
  FLOG(INFO);

  for (auto vertex: vertexes) {
    vulkan.shaderStageInfoList.emplace_back(
        vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits::eVertex)
            .setModule(vertex)
            .setPName("main"));
  }
  for (auto fragment: fragments) {
    vulkan.shaderStageInfoList.emplace_back(
        vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits::eFragment)
            .setModule(fragment)
            .setPName("main")
    );
  }
}

void vlk::VulkanDrawableObject::preparePipeline() {
  FLOG(INFO) << "INCOMPLETE" << std::endl;
// TODO do not know what to do
  // TODO OPTION EARLY shaderStageInfoList
  // TODO OPTION AFTER
  /*
  VulkanPipelineModule *pipelineModule = vulkanModule->getPipelineModule();

  auto __pipeline_layout = pipelineModule->preparePipelineLayout(vulkanObjects.textures,
                                  vulkanObjects.descLayout);

  vulkanObjects.pipeline = pipelineModule->prepareGraphicPipeline(
          vulkanObjects.shaderStageInfoList,
          __pipeline_layout,
          __render_pass);
  */

}

void vlk::VulkanDrawableObject::prepare(vlk::Camera *camera) {
  FLOG(INFO);
  this->vulkanModule->prepareCubeDataBuffers(
      camera, gameObject);

  prepareResources();
  prepareDescriptors();

}

void vlk::VulkanDrawableObject::prepareResourceShaders() {
  FLOG(INFO);

  std::vector<vk::ShaderModule> vertexes = this->vulkanModule->getShaderModule()->prepareShaderFromFiles(
      gameObject->getVertexShaderFiles());
  std::vector<vk::ShaderModule> fragments = this->vulkanModule->getShaderModule()->prepareShaderFromFiles(
      gameObject->getFragmentShaderFiles());
  std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo;
  this->internalPrepareShaders(vertexes, fragments);
}

vlk::VulkanDrawableObject::VulkanDrawableObject(vlk::VulkanModule *vulkanModule, vlk::GameObject *gameObject)
    : vulkanModule{vulkanModule},
      gameObject(gameObject) {

}

void vlk::VulkanDrawableObject::update() {
  FLOG(INFO) << "INCOMPLETE" << std::endl;

}

void vlk::VulkanDrawableObject::render() {
  FLOG(INFO) << "INCOMPLETE" << std::endl;

}

void vlk::VulkanDrawableObject::buildDrawCommandBuffer(
    vlk::SwapchainImageResources &swapchainImageResources,
    const vk::Viewport *viewport,
    const vk::Rect2D *scissor,
    const vk::RenderPass &renderPass) {
  FLOG(INFO);

  // TODO Command Buffer should be local

  // TODO ShaderStageInfoList should be local too

  // Initialize the Subordinate Command Buffer with

  auto const inheritanceInfo = vk::CommandBufferInheritanceInfo(renderPass,
                                                                0,
                                                                swapchainImageResources.framebuffer);
  auto const commandInfo = vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse
                    | vk::CommandBufferUsageFlagBits::eRenderPassContinue)
      .setPInheritanceInfo(&inheritanceInfo);
  vulkan.commandBuffer.begin(commandInfo);

  // TODO decide about pipelineLayout;
  auto __pipeline_layout = vulkanModule->getPipelineModule()
      ->preparePipelineLayout(vulkan.textures,
                              vulkan.descLayout);

  vulkan.pipeline = vulkanModule->getPipelineModule()->prepareGraphicPipeline(vulkan.shaderStageInfoList,
                                                                                     __pipeline_layout,
                                                                                     renderPass);
  //vk::Pipeline pipeline = createSubCommandBufferPipeline();
  vulkan.commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vulkan.pipeline);
  //commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->globalPipeline);
  vulkan.commandBuffer.setViewport(0, 1, viewport);
  vulkan.commandBuffer.setScissor(0, 1, scissor);

  //auto renderPass = vk::SubPass
  //commandBuffer.beginRenderPass(this->render_pass, vk::SubpassContents::eSecondaryCommandBuffers);

  vulkan.commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                                 __pipeline_layout,
                                                 0,
                                                 1,
                                                 &(swapchainImageResources.descriptor_set),
                                                 0,
                                                 nullptr);

  vulkan.commandBuffer.draw(12 * 3, 1, 0, 0);
  vulkan.commandBuffer.end();
}

void vlk::VulkanDrawableObject::prepareResources() {
  FLOG(INFO);

  prepareResourceBuffers();
  prepareResourceShaders();

}

void vlk::VulkanDrawableObject::prepareResourceBuffers() {
  FLOG(INFO);

  for (std::string &textureFile : gameObject->getTextureFiles()) {
    this->vulkanModule->prepareTexture(textureFile);
  }
}

void vlk::VulkanDrawableObject::prepareDescriptors() {
  FLOG(INFO);
}
