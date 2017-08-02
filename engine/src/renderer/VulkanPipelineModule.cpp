//
// Created by crypt on 26/07/17.
//

#include "renderer/VulkanModule.hpp"

vlk::VulkanPipelineModule::VulkanPipelineModule(vk::Device *device) : device(device) {
}

vk::Pipeline vlk::VulkanPipelineModule::
preparePipeline(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList,
                vk::PipelineLayout &__pipeline_layout,
                vk::RenderPass __render_pass) {
    vk::Pipeline resultPipeline;

    vk::PipelineCacheCreateInfo const pipelineCacheInfo;
    auto result = this->device->createPipelineCache(&pipelineCacheInfo, nullptr, &pipelineCache);
    VERIFY(result == vk::Result::eSuccess);

    vk::PipelineVertexInputStateCreateInfo const vertexInputInfo;

    auto const inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo().setTopology(
            vk::PrimitiveTopology::eTriangleList);

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

    vk::DynamicState const dynamicStates[2] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

    auto const dynamicStateInfo = vk::PipelineDynamicStateCreateInfo().setPDynamicStates(
            dynamicStates).setDynamicStateCount(2);

    auto const pipeline = vk::GraphicsPipelineCreateInfo()
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

    result = device->createGraphicsPipelines(pipelineCache, 1, &pipeline, nullptr, &resultPipeline);
    VERIFY(result == vk::Result::eSuccess);

// TODO Understand this call and replicate it
// device->destroyShaderModule(fragShaderModule, nullptr);
// device->destroyShaderModule(vertShaderModule, nullptr);

    return
            resultPipeline;
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

vk::PipelineLayout vlk::VulkanPipelineModule::preparePipelineLayout(std::vector<texture_object> &textures,
                                                                    vk::DescriptorSetLayout &descLayout) {
    vk::DescriptorSetLayoutBinding const layout_bindings[2] = {
            vk::DescriptorSetLayoutBinding()
                    .setBinding(0)
                    .setDescriptorType(
                            vk::DescriptorType::eUniformBuffer)
                    .setDescriptorCount(1)
                    .setStageFlags(vk::ShaderStageFlagBits::eVertex)
                    .setPImmutableSamplers(nullptr),
            vk::DescriptorSetLayoutBinding()
                    .setBinding(1)
                    .setDescriptorType(
                            vk::DescriptorType::eCombinedImageSampler)
                    .setDescriptorCount((uint32_t) textures.size())
                    .setStageFlags(
                            vk::ShaderStageFlagBits::eFragment)
                    .setPImmutableSamplers(nullptr)};

    auto const descriptor_layout = vk::DescriptorSetLayoutCreateInfo().setBindingCount(2).setPBindings(layout_bindings);


    auto result = device->createDescriptorSetLayout(&descriptor_layout, nullptr, &descLayout);
    VERIFY(result == vk::Result::eSuccess);

    auto const pPipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo().setSetLayoutCount(1).setPSetLayouts(
            &descLayout);

    vk::PipelineLayout resultPipelineLayout;
    result = device->createPipelineLayout(&pPipelineLayoutCreateInfo, nullptr, &resultPipelineLayout);
    VERIFY(result == vk::Result::eSuccess);
    return resultPipelineLayout;
}