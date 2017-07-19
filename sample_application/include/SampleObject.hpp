//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
#define VULKAN_ENGINE_CPP_SAMPLECUBE_HPP


#include <game/GameObject.hpp>

class SampleObject : public vlk::GameObject {

public:
    SampleObject();

    void rotateLeft();

    void rotateRight();

protected:
    static float g_vertex_buffer_data[];

    static float g_uv_buffer_data[];

    float spinningStep = 0.2;

};


#endif //VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
