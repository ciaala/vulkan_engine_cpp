//
// Created by crypt on 17/07/17.
//

#ifndef VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP
#define VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP

#include <vulkan/vulkan.hpp>
#include <vulkan/vk_sdk_platform.h>
#include "MemoryModule.hpp"
#include "CommonMacro.hpp"

namespace vlk {
    struct texture_object {
        vk::Sampler sampler;

        vk::Image image;
        vk::ImageLayout imageLayout{vk::ImageLayout::eUndefined};

        vk::MemoryAllocateInfo mem_alloc;
        vk::DeviceMemory mem;
        vk::ImageView view;

        int32_t tex_width{0};
        int32_t tex_height{0};
    };

    class TextureModule {
    public:
        void
        prepareTextureImage(const char *filename, texture_object *tex_obj, vk::ImageTiling tiling,
                            vk::ImageUsageFlags usage,
                            vk::MemoryPropertyFlags required_props);

        bool
        loadTexture(const char *filename, uint8_t *rgba_data, vk::SubresourceLayout *layout, int32_t *width,
                    int32_t *height);
        TextureModule(vk::Device *device, MemoryModule *memoryModule);


        void destroyTextureImage(texture_object *tex_objs);


        void setImageLayout(vk::CommandBuffer *cmd, vk::Image image, vk::ImageAspectFlags aspectMask, vk::ImageLayout oldLayout,
                                    vk::ImageLayout newLayout, vk::AccessFlags srcAccessMask,
                                    vk::PipelineStageFlags src_stages, vk::PipelineStageFlags dest_stages);
    private:
        vk::Device *device;
        MemoryModule *memoryModule;
    };
}

#endif //VULKAN_ENGINE_CPP_TEXTUREMODULE_HPP
