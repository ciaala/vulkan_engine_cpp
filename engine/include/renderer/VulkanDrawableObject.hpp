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
    std::vector<vk::DescriptorSetLayout> descriptorSetLayoutList;
    vk::Pipeline pipeline;
    std::vector<vk::Buffer> vertices;
    vk::Buffer index;
    std::vector<vk::DescriptorSet> descriptorSets;
    std::vector<uint32_t> vertexCount;
    uint32_t indexCount;
    vk::Buffer uniformBuffer;
    vk::PipelineLayout pipelineLayout;
    vk::RenderPass renderPass;
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
  void prepareResourceBuffers(std::shared_ptr<vk::CommandBuffer> commandBuffer);
  void prepareResourceShaders(std::shared_ptr<vk::CommandBuffer> ptr);
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
  void buildDrawCommandBuffer(vlk::Camera *camera,
                              vk::Framebuffer &framebuffer,
                              uint32_t width,
                              uint32_t height);
  // NEW API
  void  setCommandBuffer(vk::CommandBuffer *commandBuffer);


  GameObject *getGameObject();
  void prepareRenderPass();

  void prepareVulkanResources(std::shared_ptr<vk::CommandBuffer> commandBuffer);
  bool isVulkanResourcesReady;
};

}

#endif //VULKAN_ENGINE_CPP_VULKANDRAWABLEOBJECT_H
