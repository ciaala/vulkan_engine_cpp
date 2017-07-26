//
// Created by crypt on 17/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SHADERMODULE_HPP
#define VULKAN_ENGINE_CPP_SHADERMODULE_HPP


#include "RendererDefinition.hpp"

namespace vlk {
    class ShaderModule {
    private:
        vk::ShaderModule prepareShaderModule(const void *code, size_t size);

        vk::Device *device;

        char *readSpv(const char *filename, size_t *psize);

    public:

        explicit ShaderModule(vk::Device *device);

        vk::ShaderModule readAndPrepare(const char *filename);

        std::vector<vk::ShaderModule> prepareShaderFromFiles(std::vector<std::string> filenames);
    };
}

#endif //VULKAN_ENGINE_CPP_SHADERMODULE_HPP
