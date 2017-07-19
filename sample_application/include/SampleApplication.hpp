//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
#define VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP

#include <string>
#include "core/Application.hpp"


class SampleApplication : public Application {
private:
    std::string applicationName;
public:
    SampleApplication();
    static const float g_uv_buffer_data[];
    static const float g_vertex_buffer_data[];
    virtual const float* getBufferData();
    virtual const float* getUVBufferData();
    virtual std::string getName();
    ~SampleApplication() {};
};


#endif //VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
