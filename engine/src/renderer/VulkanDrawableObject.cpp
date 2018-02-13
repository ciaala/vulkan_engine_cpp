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
  FLOG(INFO) << "preparing vertexes: " << vertexes.size() << ", fragments: " << fragments.size();

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
  prepareResourceBuffers();
  prepareResourceShaders();
  prepareBuffers(camera, gameObject);

  prepareDescriptors();
  isPrepared = true;
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

vlk::VulkanDrawableObject::VulkanDrawableObject(vlk::VulkanModule *vulkanModule,
                                                vlk::GameObject *gameObject)
    : vulkanModule{vulkanModule},
      gameObject(gameObject),
      isPrepared(false){
  FLOG(INFO);
}

void somethingElse() {
  FLOG(ERROR);

  /*
}

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
  const auto commandBuffer = vulkan.commandBuffer;
  commandBuffer.begin(commandInfo);

   // TODO decide about pipelineLayout;
  auto __pipeline_layout = vulkanModule->getPipelineModule()
      ->preparePipelineLayout(vulkan.textures,
                              vulkan.descLayout);

  vulkan.pipeline = vulkanModule->getPipelineModule()->prepareGraphicPipeline(vulkan.shaderStageInfoList,
                                                                              __pipeline_layout,
                                                                              renderPass);
  //vk::Pipeline pipeline = createSubCommandBufferPipeline();
  commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vulkan.pipeline);
  //commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->globalPipeline);
  commandBuffer.setViewport(0, 1, viewport);
  commandBuffer.setScissor(0, 1, scissor);

  //auto renderPass = vk::SubPass
  //commandBuffer.beginRenderPass(this->render_pass, vk::SubpassContents::eSecondaryCommandBuffers);
  */
}
void vlk::VulkanDrawableObject::buildDrawCommandBuffer(vlk::Camera *camera) {
  FLOG(INFO);
  auto commandBuffer = vulkan.commandBuffer;
  if (! isPrepared ) {
    prepare(camera);
  }
  this->preparePipelineLayout();
  commandBuffer->bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
                                   vulkan.pipelineLayout,
                                   0,
                                   vulkan.descriptorSets.size(),
                                   vulkan.descriptorSets.data(),
                                   0,
                                   nullptr);
  if (!vulkan.vertices.empty()) {
    VkDeviceSize offsets[1] = {0};
    commandBuffer->bindVertexBuffers(0, vulkan.vertices.size(), vulkan.vertices.data(), offsets);
  }
  if (vulkan.index) {
    VkDeviceSize offset = 0;
    commandBuffer->bindIndexBuffer(vulkan.index, offset, vk::IndexType::eUint32);
  }
  if (!vulkan.index) {
    commandBuffer->draw(12 * 3, 1, 0, 0);
  } else {
    FLOG(ERROR);
    // commandBuffer->drawIndexed();
  }
  commandBuffer->end();
}

void vlk::VulkanDrawableObject::prepareResourceBuffers() {
  FLOG(INFO) << " gameObject: " << this->gameObject->getSid();
  auto textureFiles = gameObject->getTextureFiles();
  vulkan.textures.resize(textureFiles.size());
  unsigned int index = 0;
  for (std::string &textureFile : textureFiles ) {
      this->vulkanModule->prepareTextureObject(vulkan.commandBuffer, textureFile, vulkan.textures[index]);
      index ++;
  }
}

void vlk::VulkanDrawableObject::prepareDescriptors() {
  FLOG(INFO) << "AND NOW ?";

}

void vlk::VulkanDrawableObject::prepareBuffers(Camera *camera, GameObject *object) {
  FLOG(INFO);

  mat4x4 VP{0};
  mat4x4_mul(VP, camera->getProjectionMatrix(), camera->getViewMatrix());

  mat4x4 MVP{0};
  mat4x4_mul(MVP, VP, object->getModelMatrix());

  vktexcube_vs_uniform data{0};
  memcpy(data.mvp, MVP, sizeof(MVP));
  //    dumpMatrix("MVP", MVP)

  for (int32_t i = 0; i < 12 * 3; i++) {

    data.position[i][0] = object->getVertexBufferData()[i * 3];
    data.position[i][1] = object->getVertexBufferData()[i * 3 + 1];
    data.position[i][2] = object->getVertexBufferData()[i * 3 + 2];
    data.position[i][3] = 1.0f;
    data.attr[i][0] = object->getUVBufferData()[2 * i];
    data.attr[i][1] = object->getUVBufferData()[2 * i + 1];
    data.attr[i][2] = 0;
    data.attr[i][3] = 0;
  }

  makeVertexBufferFromData(data);
}
void vlk::VulkanDrawableObject::makeVertexBufferFromData(vlk::vktexcube_vs_uniform &data) {
  this->vulkanModule->makeVertexBuffer(data, vulkan.uniformBuffer);
}
vk::DeviceMemory& vlk::VulkanDrawableObject::getUniformMemory() {
  return uniforMemory;
}

void vlk::VulkanDrawableObject::setCommandBuffer(vk::CommandBuffer *commandBuffer) {
  this->vulkan.commandBuffer = commandBuffer;
}
vlk::GameObject *vlk::VulkanDrawableObject::getGameObject() {
  return gameObject;
}

void vlk::VulkanDrawableObject::preparePipelineLayout() {
  FLOG(INFO);
  vulkanModule->getDescriptorModule()
      ->prepareDescriptorSet(vulkan.descLayout, vulkan.textures, vulkan.descriptorSets, vulkan.uniformBuffer);
  vulkanModule->getPipelineModule()
      ->preparePipelineLayout(vulkan.textures, vulkan.descLayout, vulkan.pipelineLayout);
}
