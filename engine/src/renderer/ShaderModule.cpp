//
// Created by crypt on 17/07/17.
//

#include <iostream>
#include "renderer/ShaderModule.hpp"
#include "core/CommonMacro.hpp"

vk::ShaderModule vlk::ShaderModule::prepareShaderModule(const void *code, size_t size) {

    auto const moduleCreateInfo = vk::ShaderModuleCreateInfo().setCodeSize(size).setPCode((uint32_t const *) code);

    vk::ShaderModule module;
    auto result = device->createShaderModule(&moduleCreateInfo, nullptr, &module);
    VERIFY(result == vk::Result::eSuccess);

    return module;
}

vlk::ShaderModule::ShaderModule(vk::Device *device) {
    FLOG(INFO);
    this->device = device;
}

vk::ShaderModule vlk::ShaderModule::readAndPrepare(const char *filename) {
    FLOG(INFO) << "Loading: shader " << filename;
    size_t size;
    auto code = this->readSpv(filename, &size);
    if (code != nullptr) {
        return this->prepareShaderModule(code, size);
    }
    LOG(ERROR) << "Loading shader from file: '" << filename << "' has failed" << std::endl;
    return nullptr;
}

char *vlk::ShaderModule::readSpv(const char *filename, size_t *psize) {
    FLOG(INFO) << "Reading file: " << filename;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return nullptr;
    }
    // TODO Consider using ftello which could be configured to be 64bit
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

std::vector<vk::ShaderModule> vlk::ShaderModule::prepareShaderFromFiles(std::vector<std::string> filenames) {
    std::vector<vk::ShaderModule> shaders;
    for (auto &&filename : filenames) {
        shaders.emplace_back(readAndPrepare(filename.c_str()));
    }
    return shaders;
}

