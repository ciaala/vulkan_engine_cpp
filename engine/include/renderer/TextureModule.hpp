//
// Created by crypt on 17/07/17.
//

#ifndef VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP
#define VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vk_sdk_platform.h>
#include "MemoryModule.hpp"
#include "core/CommonMacro.hpp"
#include "RendererDefinition.hpp"

namespace vlk {

class TextureModule {

  // FIELDS
 private:
  vk::Device *device;
  MemoryModule *memoryModule;
  vk::FormatProperties textureFormatProperties;
  vk::Format const textureFormat = vk::Format::eR8G8B8A8Unorm;
  vk::PhysicalDevice *gpu;
  bool useStagingBuffer;
 private:
  void
  prepareTextureImage(const char *filename,
                      TextureObject &tex_obj,
                      vk::ImageTiling tiling,
                      vk::ImageUsageFlags usage,
                      vk::MemoryPropertyFlags required_props);

  bool
  loadTexture(const char *filename,
              uint8_t *rgba_data,
              vk::SubresourceLayout *layout,
              int32_t *width,
              int32_t *height);

  void destroyTextureImage(TextureObject *tex_objs);

  void setImageLayout(const vk::CommandBuffer *commandBuffer,
                      vk::Image image,
                      vk::ImageAspectFlags aspectMask,
                      vk::ImageLayout oldLayout,
                      vk::ImageLayout newLayout,
                      vk::AccessFlags srcAccessMask,
                      vk::PipelineStageFlags srcStages,
                      vk::PipelineStageFlags destStages);


  // CONSTRUCTOR & DESTRUCTOR


 private:
  void makeTextureWithoutStagingBuffer(const vk::CommandBuffer *commandBuffer,
                                         const char *textureFile,
                                         vlk::TextureObject &currentTexture);

  void makeTextureWithStagingBuffer(const vk::CommandBuffer *commandBuffer,
                                      const char *textureFile,
                                      vlk::TextureObject &currentTexture);
 public:
  TextureModule(vk::Device *device, vk::PhysicalDevice *, MemoryModule *memoryModule);

  // PUBLIC METHODS

  void makeTexture(vk::CommandBuffer *commandBuffer,
                                       const char *textureFile,
                                       const vk::Format &texFormat,
                                       TextureObject &currentTexture);

  bool loadTextureInfo(const char *filename, int32_t *width, int32_t *height);
  void createImageView(const vk::Format &texFormat, vlk::TextureObject &currentTexture) ;

  };
}

#endif //VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP
