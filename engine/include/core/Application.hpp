//
// Created by crypt on 15/07/17.
//
#include <string>

#ifndef VULKAN_ENGINE_CPP_APPLICATION_H
#define VULKAN_ENGINE_CPP_APPLICATION_H

class Application {
public:
    virtual const float *getUVBufferData() = 0;

    virtual const float *getBufferData() = 0;

    virtual std::string getName() = 0;
};

#endif //VULKAN_ENGINE_CPP_APPLICATION_H
