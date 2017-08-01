//
// Created by crypt on 17/07/17.
//

#include "../../include/renderer/TextureModule.hpp"
#include "../../include/core/CommonMacro.hpp"

void vlk::TextureModule::prepareTextureImage(const char *filename,
                                             texture_object &tex_obj,
                                             vk::ImageTiling tiling,
                                             vk::ImageUsageFlags usage,
                                             vk::MemoryPropertyFlags required_props) {
    int32_t tex_width;
    int32_t tex_height;
    if (!loadTexture(filename, nullptr, nullptr, &tex_width, &tex_height)) {
        ERR_EXIT("Failed to load textures", "Load Texture Failure");
    }

    tex_obj.tex_width = tex_width;
    tex_obj.tex_height = tex_height;

    auto const image_create_info = vk::ImageCreateInfo()
            .setImageType(vk::ImageType::e2D)
            .setFormat(vk::Format::eR8G8B8A8Unorm)
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
    VERIFY(pass == true);

    result = device->allocateMemory(&tex_obj.mem_alloc, nullptr, &(tex_obj.mem));
    VERIFY(result == vk::Result::eSuccess);


    // TODO investigate this void value
    // result =
    device->bindImageMemory(tex_obj.image, tex_obj.mem, 0);
    //VERIFY(result == vk::Result::eSuccess);

    if (required_props & vk::MemoryPropertyFlagBits::eHostVisible) {
        auto const subres =
                vk::ImageSubresource().setAspectMask(vk::ImageAspectFlagBits::eColor).setMipLevel(0).setArrayLayer(0);
        vk::SubresourceLayout layout;
        device->getImageSubresourceLayout(tex_obj.image, &subres, &layout);

        auto data = device->mapMemory(tex_obj.mem, 0, tex_obj.mem_alloc.allocationSize);
// TODO investigate this result type
//        VERIFY(data.result == vk::Result::eSuccess);
        // TODO investigate if an api change has happened here
        if (!loadTexture(filename, (uint8_t *) data, &layout, &tex_width, &tex_height)) {
            fprintf(stderr, "Error loading texture: %s\n", filename);
        }

        device->unmapMemory(tex_obj.mem);
    }

    tex_obj.imageLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
}

bool
vlk::TextureModule::loadTexture(const char *filename,
                                uint8_t *rgba_data,
                                vk::SubresourceLayout *layout,
                                int32_t *width,
                                int32_t *height) {
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
    if (cPtr == nullptr || strncmp(header, "255\n", 3)) {
        fclose(fPtr);
        return false;
    }

    for (int y = 0; y < *height; y++) {
        uint8_t *rowPtr = rgba_data;

        for (int x = 0; x < *width; x++) {
            size_t s = fread(rowPtr, 3, 1, fPtr);
            (void) s;
            rowPtr[3] = 255; /* Alpha of 1 */
            rowPtr += 4;
        }

        rgba_data += layout->rowPitch;
    }

    fclose(fPtr);
    return true;
}

void vlk::TextureModule::destroyTextureImage(vlk::texture_object *tex_objs) {
    // clean up staging resources
    device->freeMemory(tex_objs->mem, nullptr);
    device->destroyImage(tex_objs->image, nullptr);
}

vlk::TextureModule::TextureModule(vk::Device *device, vlk::MemoryModule *memoryModule) {

    this->device = device;
    this->memoryModule = memoryModule;
}

void vlk::TextureModule::setImageLayout(const vk::CommandBuffer *cmd,
                                        vk::Image image,
                                        vk::ImageAspectFlags aspectMask,
                                        vk::ImageLayout oldLayout,
                                        vk::ImageLayout newLayout,
                                        vk::AccessFlags srcAccessMask,
                                        vk::PipelineStageFlags src_stages,
                                        vk::PipelineStageFlags dest_stages) {

    assert(cmd != nullptr);

    auto DstAccessMask = [](vk::ImageLayout const &layout) {
        vk::AccessFlags flags;

        switch (layout) {
            case vk::ImageLayout::eTransferDstOptimal:
                // Make sure anything that was copying from this image has
                // completed
                flags = vk::AccessFlagBits::eTransferWrite;
                break;
            case vk::ImageLayout::eColorAttachmentOptimal:
                flags = vk::AccessFlagBits::eColorAttachmentWrite;
                break;
            case vk::ImageLayout::eDepthStencilAttachmentOptimal:
                flags = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
                break;
            case vk::ImageLayout::eShaderReadOnlyOptimal:
                // Make sure any Copy or CPU writes to image are flushed
                flags = vk::AccessFlagBits::eShaderRead | vk::AccessFlagBits::eInputAttachmentRead;
                break;
            case vk::ImageLayout::eTransferSrcOptimal:
                flags = vk::AccessFlagBits::eTransferRead;
                break;
            case vk::ImageLayout::ePresentSrcKHR:
                flags = vk::AccessFlagBits::eMemoryRead;
                break;
            default:
                break;
        }

        return flags;
    };

    auto const barrier = vk::ImageMemoryBarrier()
            .setSrcAccessMask(srcAccessMask)
            .setDstAccessMask(DstAccessMask(newLayout))
            .setOldLayout(oldLayout)
            .setNewLayout(newLayout)
            .setSrcQueueFamilyIndex(0)
            .setDstQueueFamilyIndex(0)
            .setImage(image)
            .setSubresourceRange(vk::ImageSubresourceRange(aspectMask, 0, 1, 0, 1));

    cmd->pipelineBarrier(src_stages, dest_stages, vk::DependencyFlagBits(), 0, nullptr, 0, nullptr, 1, &barrier);
}
