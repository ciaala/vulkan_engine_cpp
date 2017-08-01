//
// Created by crypt on 28/07/17.
//

#include <iostream>
#include "renderer/ModelRendererAdapter.hpp"

void
vlk::ModelRendererAdapter::prepareShaders(
        std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfo,
        std::vector<vk::ShaderModule> &vertexes,
        std::vector<vk::ShaderModule> &fragments) {

    for (auto vertex: vertexes) {
        shaderStageInfo.emplace_back(
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eVertex)
                        .setModule(vertex)
                        .setPName("main"));
    }
    for (auto fragment: fragments) {
        shaderStageInfo.emplace_back(
                vk::PipelineShaderStageCreateInfo()
                        .setStage(vk::ShaderStageFlagBits::eFragment)
                        .setModule(fragment)
                        .setPName("main")
        );
    }
}