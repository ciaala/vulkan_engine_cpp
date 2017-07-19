//
// Created by crypt on 17/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SHADERMODULE_HPP
#define VULKAN_ENGINE_CPP_SHADERMODULE_HPP


#include <vulkan/vulkan.hpp>

namespace vlk {
    class ShaderModule {
    public:
        vk::ShaderModule prepareShaderModule(const void *code, size_t size);

        vk::Device *device;
        char *readSpv(const char *filename, size_t *psize);

        ShaderModule(vk::Device *device);
    };
}

#endif //VULKAN_ENGINE_CPP_SHADERMODULE_HPP
