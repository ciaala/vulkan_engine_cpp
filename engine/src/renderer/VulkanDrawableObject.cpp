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
                                  vulkanObjects.descriptorSetLayout);

  vulkanObjects.pipeline = pipelineModule->prepareGraphicPipeline(
          vulkanObjects.shaderStageInfoList,
          __pipeline_layout,
          __render_pass);
  */

}

void vlk::VulkanDrawableObject::prepare(vlk::Camera *camera) {
  FLOG(INFO);
  if (1 == 0) {
    prepareRenderPass();
  }
//  prepareResourceBuffers();
//  prepareResourceShaders();
  prepareBuffers(camera, gameObject);

  prepareDescriptors();
  isPrepared = true;
}

void vlk::VulkanDrawableObject::prepareResourceShaders(std::shared_ptr<vk::CommandBuffer> commandBuffer) {
  FLOG(INFO);

  std::vector<vk::ShaderModule> vertexes = this->vulkanModule->getShaderModule()->prepareShaderFromFiles(
      gameObject->getVertexShaderFiles());
  std::vector<vk::ShaderModule> fragments = this->vulkanModule->getShaderModule()->prepareShaderFromFiles(
      gameObject->getFragmentShaderFiles());
  std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfo;
  this->internalPrepareShaders(vertexes, fragments);
}

vlk::VulkanDrawableObject::VulkanDrawableObject(
    vlk::VulkanModule *vulkanModule,
    vlk::GameObject *gameObject)
    : vulkanModule{vulkanModule},
      gameObject(gameObject),
      isPrepared(false),
      isVulkanResourcesReady{false}{
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
                              vulkan.descriptorSetLayout);

  vulkan.pipeline = vulkanModule->getPipelineModule()->prepareGraphicPipeline(vulkan.shaderStageInfoList,
                                                                              __pipeline_layout,
                                                                              renderPass);
  //vk::Pipeline pipeline = createSubCommandBufferPipeline();
  commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, vulkan.pipeline);
  //commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->globalPipeline);
  commandBuffer.setViewport(0, 1, viewport);
  commandBuffer.setScissor(0, 1, scissor);

  //auto renderPass = vk::SubPass
  //commandBuffer.beginRenderPass(this->renderPass, vk::SubpassContents::eSecondaryCommandBuffers);
  */
}

void vlk::VulkanDrawableObject::buildDrawCommandBuffer(
    vlk::Camera *camera,
    vk::Framebuffer &framebuffer,
    uint32_t width,
    uint32_t height) {
  FLOG(INFO);
  auto commandBuffer = vulkan.commandBuffer;
  if (!isPrepared) {
    prepare(camera);
  }
  this->preparePipelineLayout();
  if ( "disable_me_2018-03-11" == "ciao") {
    auto const passInfo = vk::RenderPassBeginInfo()
        .setPNext(nullptr)
        .setRenderPass(vulkan.renderPass)
        .setFramebuffer(framebuffer)
        .setRenderArea(
            vk::Rect2D(
                vk::Offset2D(0, 0),
                vk::Extent2D(width, height)));

    commandBuffer->beginRenderPass(passInfo, vk::SubpassContents::eInline);
  }

  commandBuffer->bindDescriptorSets(
      vk::PipelineBindPoint::eGraphics,
      vulkan.pipelineLayout,
      0,
      (uint32_t) vulkan.descriptorSets.size(),
      vulkan.descriptorSets.data(),
      0,
      nullptr);
  commandBuffer->bindPipeline(vk::PipelineBindPoint::eGraphics, vulkan.pipeline);
  if (!vulkan.vertices.empty()) {
    VkDeviceSize offsets[1] = {0};
    commandBuffer->bindVertexBuffers(
        0,
        (uint32_t) vulkan.vertices.size(),
        vulkan.vertices.data(), offsets);
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

void vlk::VulkanDrawableObject::prepareResourceBuffers(std::shared_ptr<vk::CommandBuffer> commandBuffer) {
  FLOG(INFO) << " gameObject: " << this->gameObject->getSid();
  auto textureFiles = gameObject->getTextureFiles();
  vulkan.textures.resize(textureFiles.size());
  unsigned int index = 0;
  for (std::string &textureFile : textureFiles) {
    this->vulkanModule->prepareTextureObject(
        commandBuffer.get(),
        textureFile,
        vulkan.textures[index]);
    index++;
  }
}

void vlk::VulkanDrawableObject::prepareDescriptors() {
  FLOG(INFO) << "AND NOW ?";

}

void vlk::VulkanDrawableObject::prepareBuffers(
    Camera *camera,
    GameObject *object) {
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

vk::DeviceMemory &vlk::VulkanDrawableObject::getUniformMemory() {
  FLOG(INFO);
  return uniforMemory;
}

void vlk::VulkanDrawableObject::setCommandBuffer(vk::CommandBuffer *commandBuffer) {
  FLOG(INFO);
  this->vulkan.commandBuffer = commandBuffer;
}

vlk::GameObject *vlk::VulkanDrawableObject::getGameObject() {
  FLOG(INFO);
  return gameObject;
}

void vlk::VulkanDrawableObject::preparePipelineLayout() {
  FLOG(INFO);
  DescriptorModule *descriptorModule = vulkanModule->getDescriptorModule();
  descriptorModule->prepareDescriptorSetLayout(
      vulkan.textures,
      vulkan.descriptorSetLayoutList);
  descriptorModule->updateDescriptorSet(
      vulkan.textures,
      vulkan.uniformBuffer,
      vulkan.descriptorSetLayoutList,
      vulkan.descriptorSets);
  auto pipelineModule = vulkanModule->getPipelineModule();
  pipelineModule->preparePipelineLayout(
      vulkan.descriptorSetLayoutList,
      vulkan.pipelineLayout);
  pipelineModule->prepareGraphicPipeline(
      vulkan.shaderStageInfoList,
      vulkan.pipelineLayout,
      this->vulkanModule->getRenderPass(),
      vulkan.pipeline);

}
void vlk::VulkanDrawableObject::prepareRenderPass() {
  const vk::AttachmentDescription
      attachments[2] = {vk::AttachmentDescription()
                            .setFormat(this->vulkanModule->getFormat())
                            .setSamples(vk::SampleCountFlagBits::e1)
                            .setLoadOp(vk::AttachmentLoadOp::eClear)
                            .setStoreOp(vk::AttachmentStoreOp::eStore)
                            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                            .setInitialLayout(vk::ImageLayout::eUndefined)
                            .setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
                        vk::AttachmentDescription()
                            .setFormat(this->vulkanModule->getDepthFormat())
                            .setSamples(vk::SampleCountFlagBits::e1)
                            .setLoadOp(vk::AttachmentLoadOp::eClear)
                            .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                            .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                            .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                            .setInitialLayout(vk::ImageLayout::eUndefined)
                            .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)};
  vk::AttachmentReference colorReference = vk::AttachmentReference()
      .setAttachment(0)
      .setLayout(vk::ImageLayout::eColorAttachmentOptimal);
  vk::AttachmentReference depthReference = vk::AttachmentReference()
      .setAttachment(1)
      .setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  auto const subpass = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setInputAttachmentCount(0)
      .setPInputAttachments(nullptr)
      .setColorAttachmentCount(1)
      .setPColorAttachments(&colorReference)
      .setPResolveAttachments(0)
      .setPDepthStencilAttachment(&depthReference)
      .setPreserveAttachmentCount(0)
      .setPPreserveAttachments(nullptr);

  auto const rp_info = vk::RenderPassCreateInfo()
      .setAttachmentCount(2)
      .setPAttachments(attachments)
      .setSubpassCount(1)
      .setPSubpasses(&subpass)
      .setDependencyCount(0)
      .setPDependencies(nullptr);

  auto result = vulkanModule->getDevice()->createRenderPass(
      &rp_info,
      nullptr,
      &vulkan.renderPass);
  VERIFY(result == vk::Result::eSuccess);
}
void vlk::VulkanDrawableObject::prepareVulkanResources(std::shared_ptr< vk::CommandBuffer> commandBuffer) {
  if ( !isVulkanResourcesReady ) {
    prepareResourceBuffers(commandBuffer);
    prepareResourceShaders(commandBuffer);
    isVulkanResourcesReady = true;
  }
}
