//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
#define VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP

#include <string>
#include "game/Application.hpp"
#include "SampleInputController.hpp"


class SampleApplication : public Application {
private:
    std::string applicationName;
public:
    SampleApplication();

    static const float g_uv_buffer_data[];
    static const float g_vertex_buffer_data[];

    virtual const float *getBufferData() override;

    virtual const float *getUVBufferData() override;

    virtual std::string getName() override;

    virtual InputController *getInputController() override;

    ~SampleApplication() {};

    SampleGame *game;

    virtual vlk::GameObject *getWorld() override;

    SampleInputController *inputController;
};


#endif //VULKAN_ENGINE_CPP_SAMPLEAPPLICATION_HPP
