//
// Created by crypt on 17/07/17.
//

#include <iostream>
#include "renderer/TextureModule.hpp"
#include "../utility/Utility.hpp"

void vlk::TextureModule::prepareTextureImage(const char *filename,
                                             TextureObject &tex_obj,
                                             vk::ImageTiling tiling,
                                             vk::ImageUsageFlags usage,
                                             vk::MemoryPropertyFlags required_props) {
  FLOG(INFO) << "Loading: texture image " << Utility::getAbsolutePath(filename).string();
  int32_t tex_width;
  int32_t tex_height;
  if (!loadTextureInfo(filename, &tex_width, &tex_height)) {
    auto message = std::string("Failed to load textures: ") + Utility::getAbsolutePath(filename).string();
    ERR_EXIT(message.c_str(), "Load Texture Failure");
  }

  tex_obj.tex_width = tex_width;
  tex_obj.tex_height = tex_height;

  auto const image_create_info = vk::ImageCreateInfo()
      .setImageType(vk::ImageType::e2D)
      .setFormat(textureFormat)
      .setExtent({(uint32_t) tex_width, (uint32_t) tex_height, 1})
      .setMipLevels(1)
      .setArrayLayers(1)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setTiling(tiling)
      .setUsage(usage)
      .setSharingMode(vk::SharingMode::eExclusive)
      .setQueueFamilyIndexCount(0)
      .setPQueueFamilyIndices(nullptr)
      .setInitialLayout(vk::ImageLayout::ePreinitialized);

  auto result = device->createImage(&image_create_info, nullptr, &tex_obj.image);
  VERIFY(result == vk::Result::eSuccess);

  vk::MemoryRequirements mem_reqs;
  device->getImageMemoryRequirements(tex_obj.image, &mem_reqs);

  tex_obj.mem_alloc.setAllocationSize(mem_reqs.size);
  tex_obj.mem_alloc.setMemoryTypeIndex(0);

  auto pass = memoryModule->memoryTypeFromProperties(mem_reqs.memoryTypeBits, required_props,
                                                     &tex_obj.mem_alloc.memoryTypeIndex);
  VERIFY(pass);

  result = device->allocateMemory(&tex_obj.mem_alloc, nullptr, &(tex_obj.mem));
  VERIFY(result == vk::Result::eSuccess);


  // TODO investigate this void value
  // result =
  device->bindImageMemory(tex_obj.image, tex_obj.mem, 0);
  //VERIFY(result == vk::Result::eSuccess);

  if (required_props & vk::MemoryPropertyFlagBits::eHostVisible) {
    FLOG(INFO) << "loading HostVisibile";
    auto const subres =
        vk::ImageSubresource().setAspectMask(vk::ImageAspectFlagBits::eColor).setMipLevel(0).setArrayLayer(0);
    vk::SubresourceLayout layout;
    device->getImageSubresourceLayout(tex_obj.image, &subres, &layout);

    auto data = device->mapMemory(tex_obj.mem, 0, tex_obj.mem_alloc.allocationSize);
    if (!loadTexture(filename, (uint8_t *) data, &layout, &tex_width, &tex_height)) {
      fprintf(stderr, "Error loading texture: %s\n", filename);
    }

    device->unmapMemory(tex_obj.mem);
  }
//TODO 2018-03-11 switching Shader Optimal to Shader general
  tex_obj.imageLayout = vk::ImageLayout::eGeneral;
}
bool
vlk::TextureModule::loadTextureInfo(const char *filename,
                                    int32_t *width,
                                    int32_t *height) {
  FLOG(INFO);
  return this->loadTexture(filename, nullptr, nullptr, width, height);
}

bool
vlk::TextureModule::loadTexture(const char *filename,
                                uint8_t *rgba_data,
                                vk::SubresourceLayout *layout,
                                int32_t *width,
                                int32_t *height) {

  if (rgba_data != nullptr) {
    FLOG(INFO) << "Loading: " << Utility::getAbsolutePath(filename);
  }
  FILE *fPtr = fopen(filename, "rb");
  if (!fPtr) {
    return false;
  }

  char header[256];
  char *cPtr = fgets(header, 256, fPtr);  // P6
  if (cPtr == nullptr || strncmp(header, "P6\n", 3)) {
    fclose(fPtr);
    return false;
  }

  do {
    cPtr = fgets(header, 256, fPtr);
    if (cPtr == nullptr) {
      fclose(fPtr);
      return false;
    }
  } while (!strncmp(header, "#", 1));

  sscanf(header, "%u %u", width, height);
  if (rgba_data == nullptr) {
    fclose(fPtr);
    return true;
  }

  char *result = fgets(header, 256, fPtr);  // Format
  VERIFY(result != nullptr);
  if (cPtr == nullptr || strncmp(header, "255\n", 3) != 0) {
    fclose(fPtr);
    return false;
  }

  for (int y = 0; y < *height; y++) {
    uint8_t *rowPtr = rgba_data;

    for (int x = 0; x < *width; x++) {
      size_t s = fread(rowPtr, 3, 1, fPtr);
      // (void) s;
      rowPtr[3] = 255; /* Alpha of 1 */
      rowPtr += 4;
    }
    rgba_data += layout->rowPitch;
  }

  fclose(fPtr);
  return true;
}

void vlk::TextureModule::destroyTextureImage(vlk::TextureObject *tex_objs) {
  FLOG(INFO);

  // clean up staging resources
  device->freeMemory(tex_objs->mem, nullptr);
  device->destroyImage(tex_objs->image, nullptr);
}

vlk::TextureModule::TextureModule(vk::Device *device,
                                  vk::PhysicalDevice *gpu,
                                  vlk::MemoryModule *memoryModule,
                                  vk::Format &textureFormat) :
    gpu(gpu),
    device(device),
    memoryModule(memoryModule),
    useStagingBuffer{true},
    textureFormat{textureFormat}{
  FLOG(INFO);
  gpu->getFormatProperties(textureFormat, &textureFormatProperties);
}

auto DstAccessMask = [](vk::ImageLayout const &layout) {
  FLOG(INFO);
  vk::AccessFlags flags;
  switch (layout) {
    case vk::ImageLayout::eTransferDstOptimal:
      // Make sure anything that was copying from this image has
      // completed
      flags = vk::AccessFlagBits::eTransferWrite;
      break;
    case vk::ImageLayout::eColorAttachmentOptimal:flags = vk::AccessFlagBits::eColorAttachmentWrite;
      break;
    case vk::ImageLayout::eDepthStencilAttachmentOptimal:flags = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
      break;
    case vk::ImageLayout::eShaderReadOnlyOptimal:
      // Make sure any Copy or CPU writes to image are flushed
      flags = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead;
      break;
    case vk::ImageLayout::eTransferSrcOptimal:flags = vk::AccessFlagBits::eTransferRead;
      break;
    case vk::ImageLayout::ePresentSrcKHR:flags = vk::AccessFlagBits::eMemoryRead;
      break;
    default:break;
  }
  return flags;
};

void vlk::TextureModule::setImageLayout(const vk::CommandBuffer *commandBuffer,
                                        vk::Image image,
                                        vk::ImageAspectFlags aspectMask,
                                        vk::ImageLayout oldLayout,
                                        vk::ImageLayout newLayout,
                                        vk::AccessFlags srcAccessMask,
                                        vk::PipelineStageFlags srcStages,
                                        vk::PipelineStageFlags destStages) {
  FLOG(INFO);

  auto const imageMemoryBarrier = vk::ImageMemoryBarrier()
      .setSrcAccessMask(srcAccessMask)
      .setDstAccessMask(DstAccessMask(newLayout))
      .setOldLayout(oldLayout)
      .setNewLayout(newLayout)
      .setSrcQueueFamilyIndex(0)
      .setDstQueueFamilyIndex(0)
      .setImage(image)
      .setSubresourceRange(vk::ImageSubresourceRange(aspectMask, 0, 1, 0, 1));

  const int memoryBarrierCount = 0;
  const int bufferMemoryBarrierCount = 0;
  const vk::BufferMemoryBarrier *bufferMemoryBarriers = nullptr;
  const vk::MemoryBarrier *memoryBarriers = nullptr;
  const int imageMemoryBarrierCount = 1;
  commandBuffer->pipelineBarrier(srcStages,
                                 destStages,
                                 vk::DependencyFlagBits(),

                                 memoryBarrierCount,
                                 memoryBarriers,

                                 bufferMemoryBarrierCount,
                                 bufferMemoryBarriers,

                                 imageMemoryBarrierCount,
                                 &imageMemoryBarrier);
}

void vlk::TextureModule::makeTexture(vk::CommandBuffer *commandBuffer,
                                     const char *textureFile,
                                     const vk::Format &texFormat,
                                     TextureObject &currentTexture) {
  FLOG(INFO) << textureFile;

  if ((this->textureFormatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) &&
      !this->useStagingBuffer) {
    this->makeTextureWithoutStagingBuffer(commandBuffer, textureFile, currentTexture);
  } else if (this->textureFormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) {
    this->makeTextureWithStagingBuffer(commandBuffer, textureFile, currentTexture);
  } else {
    FLOG(ERROR) << "No support for R8G8B8A8_UNORM as texture image format";
  }

  this->createImageView(texFormat, currentTexture);

}

void vlk::TextureModule::makeTextureWithStagingBuffer(const vk::CommandBuffer *commandBuffer,
                                                      const char *textureFile,
                                                      vlk::TextureObject &currentTexture) {/* Must use staging buffer to copy linear texture to optimized */

  FLOG(INFO) << textureFile;
  TextureObject staging_texture;
  this->prepareTextureImage(textureFile,
                            staging_texture,
                            vk::ImageTiling::eLinear,
                            vk::ImageUsageFlagBits::eTransferSrc,
                            vk::MemoryPropertyFlagBits::eHostVisible
                                | vk::MemoryPropertyFlagBits::eHostCoherent);

  this->prepareTextureImage(textureFile,
                            currentTexture,
                            vk::ImageTiling::eOptimal,
                            vk::ImageUsageFlagBits::eTransferDst
                                | vk::ImageUsageFlagBits::eSampled,
                            vk::MemoryPropertyFlagBits::eDeviceLocal);

  this->setImageLayout(commandBuffer,
                       staging_texture.image,
                       vk::ImageAspectFlagBits::eColor,
                       vk::ImageLayout::ePreinitialized,
                       vk::ImageLayout::eTransferSrcOptimal,
                       vk::AccessFlagBits(),
                       vk::PipelineStageFlagBits::eTopOfPipe,
                       vk::PipelineStageFlagBits::eTransfer);

  this->setImageLayout(commandBuffer,
                       currentTexture.image,
                       vk::ImageAspectFlagBits::eColor,
                       vk::ImageLayout::ePreinitialized,
                       vk::ImageLayout::eTransferDstOptimal,
                       vk::AccessFlagBits(),
                       vk::PipelineStageFlagBits::eTopOfPipe,
                       vk::PipelineStageFlagBits::eTransfer);

  auto const subResource = vk::ImageSubresourceLayers()
      .setAspectMask(vk::ImageAspectFlagBits::eColor)
      .setMipLevel(0)
      .setBaseArrayLayer(0)
      .setLayerCount(1);

  auto const copy_region =
      vk::ImageCopy()
          .setSrcSubresource(subResource)
          .setSrcOffset({0, 0, 0})
          .setDstSubresource(subResource)
          .setDstOffset({0, 0, 0})
          .setExtent(
              {(uint32_t) staging_texture.tex_width,
               (uint32_t) staging_texture.tex_height, 1});

  commandBuffer->copyImage(staging_texture.image,
                           vk::ImageLayout::eTransferSrcOptimal,
                           currentTexture.image,
                           vk::ImageLayout::eTransferDstOptimal,
                           1,
                           &copy_region);

  this->setImageLayout(commandBuffer,
                       currentTexture.image,
                       vk::ImageAspectFlagBits::eColor,
                       vk::ImageLayout::eTransferDstOptimal,
                       currentTexture.imageLayout,
                       vk::AccessFlagBits::eTransferWrite,
                       vk::PipelineStageFlagBits::eTransfer,
                       vk::PipelineStageFlagBits::eFragmentShader);

}
void vlk::TextureModule::makeTextureWithoutStagingBuffer(const vk::CommandBuffer *commandBuffer,
                                                         const char *textureFile,
                                                         vlk::TextureObject &currentTexture) {
  /* Device can texture using linear textures */
  FLOG(INFO);
  vlk::TextureObject staging_texture;
  this->prepareTextureImage(textureFile, currentTexture, vk::ImageTiling::eLinear,
                            vk::ImageUsageFlagBits::eSampled,
                            vk::MemoryPropertyFlagBits::eHostVisible
                                | vk::MemoryPropertyFlagBits::eHostCoherent);
  // Nothing in the globalPipeline needs to be complete to start, and don't allow fragment
  // shader to run until layout transition completes
  this->setImageLayout(commandBuffer,
                       currentTexture.image,
                       vk::ImageAspectFlagBits::eColor,
                       vk::ImageLayout::ePreinitialized,
                       currentTexture.imageLayout,
                       vk::AccessFlagBits::eHostWrite,
                       vk::PipelineStageFlagBits::eTopOfPipe,
                       vk::PipelineStageFlagBits::eFragmentShader);
  staging_texture.image = vk::Image();
}

void vlk::TextureModule::createImageView(const vk::Format &texFormat,
                                         vlk::TextureObject &currentTexture) {
  FLOG(INFO);
  auto samplerInfo = vk::SamplerCreateInfo()
      .setMagFilter(vk::Filter::eNearest)
      .setMinFilter(vk::Filter::eNearest)
      .setMipmapMode(vk::SamplerMipmapMode::eNearest)
      .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
      .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
      .setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
      .setMipLodBias(0.0f)
      .setAnisotropyEnable(VK_FALSE)
      .setMaxAnisotropy(1)
      .setCompareEnable(VK_FALSE)
      .setCompareOp(vk::CompareOp::eNever)
      .setMinLod(0.0f)
      .setMaxLod(0.0f)
      .setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
      .setUnnormalizedCoordinates(VK_FALSE);

  auto result = this->device->createSampler(&samplerInfo, nullptr, &(currentTexture.sampler));
  VERIFY(result == vk::Result::eSuccess);

  auto const viewInfo = vk::ImageViewCreateInfo()
      .setImage(currentTexture.image)
      .setViewType(vk::ImageViewType::e2D)
      .setFormat(texFormat)
      .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  result = this->device->createImageView(&viewInfo,
                                         nullptr,
                                         &currentTexture.view);
  VERIFY(result == vk::Result::eSuccess);
}
