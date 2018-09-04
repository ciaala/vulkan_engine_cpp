//
// Created by crypt on 19/07/17.
//

#include "SampleObject.hpp"

void SampleObject::rotateLeft() {
    this->spinningAngle -= spinningStep;
}

void SampleObject::rotateRight() {
    this->spinningAngle += spinningStep;
}

SampleObject::SampleObject(vlk::ResourceModel *resourceModel) {
    FLOG(INFO);
    this->resourceModel = resourceModel;
    this->vertexBufferData = resourceModel->getVertex();
    this->uvBufferData = resourceModel->getUV();

    this->textureFiles = resourceModel->getTextures();
    //this->textureFiles.emplace_back("sample/resources/Martini.ppm");
    this->vertexShaderFiles = resourceModel->getVertexShaders();
    this->fragmentShaderFiles = resourceModel->getFragmentShaders();
}

void SampleObject::setSpinningAngle(float spinningAngle) {
    this->spinningAngle = spinningAngle;
}

void SampleObject::setModelMatrix(mat4x4 modelMatrix) {
    mat4x4_dup(this->modelMatrix, modelMatrix);
}
