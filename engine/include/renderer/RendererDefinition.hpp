//
// Created by crypt on 16/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP
#define VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP

#include <vulkan/vulkan.hpp>

namespace vlk {
    struct vktexcube_vs_uniform {
        // Must start with MVP
        float mvp[4][4];
        float position[12 * 3][4];
        float attr[12 * 3][4];
    };

    typedef struct {
        vk::Image image;
        vk::CommandBuffer cmd;
        vk::CommandBuffer graphics_to_present_cmd;
        vk::ImageView view;
        vk::Buffer uniform_buffer;
        vk::DeviceMemory uniform_memory;
        vk::Framebuffer framebuffer;
        vk::DescriptorSet descriptor_set;
    } SwapchainImageResources;

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
}
#endif //VULKAN_ENGINE_CPP_RENDERERDEFINITION_HPP
