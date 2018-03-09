//
// Created by crypt on 26/07/17.
//

#include "renderer/VulkanModule.hpp"

vlk::VulkanPipelineModule::VulkanPipelineModule(vk::Device *device) : device(device) {
  FLOG(INFO);
}

vk::Pipeline
vlk::VulkanPipelineModule::prepareGraphicPipeline(
    std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
    vk::PipelineLayout &__pipeline_layout,
    vk::RenderPass __render_pass) {
  FLOG(INFO);
  vk::Pipeline resultPipeline;

  vk::PipelineCacheCreateInfo const pipelineCacheInfo;
  auto result = this->device->createPipelineCache(&pipelineCacheInfo, nullptr, &pipelineCache);
  VERIFY(result == vk::Result::eSuccess);

  vk::PipelineVertexInputStateCreateInfo const vertexInputInfo;

  auto const inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo()
      .setTopology(vk::PrimitiveTopology::eTriangleList);

// TODO: Where are pViewports and pScissors set?
  auto const viewportInfo = vk::PipelineViewportStateCreateInfo().setViewportCount(1).setScissorCount(1);

  auto const rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
      .setDepthClampEnable(VK_FALSE)
      .setRasterizerDiscardEnable(VK_FALSE)
      .setPolygonMode(vk::PolygonMode::eFill)
      .setCullMode(vk::CullModeFlagBits::eBack)
      .setFrontFace(vk::FrontFace::eCounterClockwise)
      .setDepthBiasEnable(VK_FALSE)
      .setLineWidth(1.0f);

  auto const multisampleInfo = vk::PipelineMultisampleStateCreateInfo();

  auto const stencilOp = vk::StencilOpState()
      .setFailOp(vk::StencilOp::eKeep)
      .setPassOp(vk::StencilOp::eKeep)
      .setCompareOp(vk::CompareOp::eAlways);

  auto const depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
      .setDepthTestEnable(VK_TRUE)
      .setDepthWriteEnable(VK_TRUE)
      .setDepthCompareOp(vk::CompareOp::eLessOrEqual)
      .setDepthBoundsTestEnable(VK_FALSE)
      .setStencilTestEnable(VK_FALSE)
      .setFront(stencilOp)
      .setBack(stencilOp);

  vk::PipelineColorBlendAttachmentState const colorBlendAttachments[1] = {
      vk::PipelineColorBlendAttachmentState().setColorWriteMask(
          vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB |
              vk::ColorComponentFlagBits::eA)};

  auto const colorBlendInfo =
      vk::PipelineColorBlendStateCreateInfo().setAttachmentCount(1).setPAttachments(colorBlendAttachments);

  std::vector<vk::DynamicState> dynamicStates = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

  auto const dynamicStateInfo = vk::PipelineDynamicStateCreateInfo()
      .setPDynamicStates(dynamicStates.data())
      .setDynamicStateCount((uint32_t) dynamicStates.size());

  auto const pipelineCreateInfo = vk::GraphicsPipelineCreateInfo()
      .setStageCount((uint32_t) shaderStageInfoList.size())
      .setPStages(shaderStageInfoList.data())
      .setPVertexInputState(&vertexInputInfo)
      .setPInputAssemblyState(&inputAssemblyInfo)
      .setPViewportState(&viewportInfo)
      .setPRasterizationState(&rasterizationInfo)
      .setPMultisampleState(&multisampleInfo)
      .setPDepthStencilState(&depthStencilInfo)
      .setPColorBlendState(&colorBlendInfo)
      .setPDynamicState(&dynamicStateInfo)
      .setLayout(__pipeline_layout)
      .setRenderPass(__render_pass);

  result = device->createGraphicsPipelines(pipelineCache, 1, &pipelineCreateInfo, nullptr, &resultPipeline);
  VERIFY(result == vk::Result::eSuccess);

// TODO Understand this call and replicate it
// device->destroyShaderModule(fragShaderModule, nullptr);
// device->destroyShaderModule(vertShaderModule, nullptr);

  return resultPipeline;
}

vlk::VulkanPipelineModule::~VulkanPipelineModule() {
  for (auto item : pipelineList) {
    device->destroyPipeline(item);
  }

  for (auto item : pipelineLayoutList) {
    device->destroyPipelineLayout(item);
  }

  device->destroyPipelineCache(pipelineCache, nullptr);

}

void vlk::VulkanPipelineModule::preparePipelineLayout(std::vector<vk::DescriptorSetLayout> &descriptorSetLayoutList,
                                                      vk::PipelineLayout &outPipelineLayout) {
  FLOG(INFO);

  auto const pPipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo()
      .setSetLayoutCount(1)
      .setPSetLayouts(&descriptorSetLayoutList[0]);

  auto result = device->createPipelineLayout(&pPipelineLayoutCreateInfo, nullptr, &outPipelineLayout);
  VERIFY(result == vk::Result::eSuccess);
}
