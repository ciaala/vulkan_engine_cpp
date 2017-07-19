//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
#define VULKAN_ENGINE_CPP_SAMPLECUBE_HPP


#include <game/GameObject.hpp>

class SampleCube : public vlk::GameObject {

public:
    void rotateLeft();

    void rotateRight();

    float spinningStep = 0.2;
};


#endif //VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
