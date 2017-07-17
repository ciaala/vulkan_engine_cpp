//
// Created by crypt on 17/07/17.
//

#include <iostream>
#include "../../include/ShaderModule.hpp"
#include "../../include/CommonMacro.hpp"

vk::ShaderModule vlk::ShaderModule::prepareShaderModule(const void *code, size_t size) {
    auto const moduleCreateInfo = vk::ShaderModuleCreateInfo().setCodeSize(size).setPCode((uint32_t const *) code);

    vk::ShaderModule module;
    auto result = device->createShaderModule(&moduleCreateInfo, nullptr, &module);
    VERIFY(result == vk::Result::eSuccess);

    return module;
}

vlk::ShaderModule::ShaderModule(vk::Device *device) {
    this->device = device;

}

char *vlk::ShaderModule::readSpv(const char *filename, size_t *psize) {

    std::cout << "Reading file: " << filename << std::endl;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return nullptr;
    }

    fseek(fp, 0L, SEEK_END);
    long int size = ftell(fp);

    fseek(fp, 0L, SEEK_SET);

    void *shader_code = malloc(size);
    size_t retval = fread(shader_code, size, 1, fp);
    VERIFY(retval == 1);

    *psize = size;

    fclose(fp);

    return (char *) shader_code;
}

