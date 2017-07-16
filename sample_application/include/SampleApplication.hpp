//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
#define VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP


#include "Application.hpp"
struct vktexcube_vs_uniform {
    // Must start with MVP
    float mvp[4][4];
    float position[12 * 3][4];
    float attr[12 * 3][4];
};
class SampleApplication : public Application {
public:
    SampleApplication();
    std::string getName();
};


#endif //VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
