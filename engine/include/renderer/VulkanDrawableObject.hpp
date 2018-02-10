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
    vk::CommandBuffer *commandBuffer;
    std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfoList;
    std::vector<TextureObject> textures;
    vk::DescriptorSetLayout descLayout;
    vk::Pipeline pipeline;
    std::vector<vk::Buffer> vertices;
    vk::Buffer index;
    std::vector<vk::DescriptorSet> descriptorSets;
    std::vector<uint32_t> vertexCount;
    uint32_t indexCount;
    vk::Buffer uniformBuffer;
    vk::PipelineLayout pipelineLayout;
  } vulkan;

  VulkanModule *vulkanModule;
  //VulkanPipelineModule *pipelineModule;
  GameObject *gameObject;
  vk::DeviceMemory uniforMemory;
  //vk::CommandBuffer *commandBuffer;
 private:
  void internalPrepareShaders(
      std::vector<vk::ShaderModule> &vertexes,
      std::vector<vk::ShaderModule> &fragments
  );

  void prepareDescriptors();
  void preparePipeline();
  void prepareResourceBuffers();
  void prepareResourceShaders();
  void prepare(vlk::Camera *camera);
  std::vector<TextureObject> &getTextures();
  bool isPrepared;
  void prepareBuffers(Camera *camera, GameObject *object);
  vk::DeviceMemory &getUniformMemory();
  vk::DescriptorSetLayout &getDescriptorSetLayout();
  void makeVertexBufferFromData(vlk::vktexcube_vs_uniform &data);
  void preparePipelineLayout();
 public:

  VulkanDrawableObject(VulkanModule *vulkanModule, GameObject *gameObject);
  void buildDrawCommandBuffer(vlk::Camera *camera);
  // NEW API
  void  setCommandBuffer(vk::CommandBuffer *commandBuffer);


  GameObject *getGameObject();
};

}

#endif //VULKAN_ENGINE_CPP_VULKANDRAWABLEOBJECT_H
