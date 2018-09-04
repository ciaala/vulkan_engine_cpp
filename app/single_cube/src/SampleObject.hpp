//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
#define VULKAN_ENGINE_CPP_SAMPLECUBE_HPP


#include <game/GameObject.hpp>
#include <resource/ResourceModel.hpp>

class SampleObject : public vlk::GameObject {

public:
    explicit SampleObject(vlk::ResourceModel *resourceModel);

    void rotateLeft();

    void rotateRight();

    void setSpinningAngle(float spinningAngle);

    void setModelMatrix(mat4x4 modelMatrix);

protected:

    float spinningStep = 0.2;
    vlk::ResourceModel *resourceModel;
};


#endif //VULKAN_ENGINE_CPP_SAMPLECUBE_HPP
