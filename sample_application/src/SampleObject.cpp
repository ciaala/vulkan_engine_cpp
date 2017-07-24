//
// Created by crypt on 19/07/17.
//

#include "../include/SampleObject.hpp"

/*
float SampleObject::g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f,  // -X side
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,  // -Z side
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,  // -Y side
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,  // +Y side
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,

        1.0f, 1.0f, -1.0f,  // +X side
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        -1.0f, 1.0f, 1.0f,  // +Z side
        -1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
};

float SampleObject::g_uv_buffer_data[] = {
        0.0f, 1.0f,  // -X side
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        1.0f, 1.0f,  // -Z side
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,  // -Y side
        1.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,  // +Y side
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,

        1.0f, 0.0f,  // +X side
        0.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,

        0.0f, 0.0f,  // +Z side
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
};
*/
void SampleObject::rotateLeft() {
    this->spinningAngle -= spinningStep;
}

void SampleObject::rotateRight() {
    this->spinningAngle += spinningStep;
}

SampleObject::SampleObject(vlk::ResourceModel *resourceModel) {
    this->resourceModel = resourceModel;
    this->vertexBufferData = resourceModel->getVertex();
    this->uvBufferData = resourceModel->getUV();

    this->textureFiles = resourceModel->getTextures();
    //this->textureFiles.emplace_back("sample_application/resources/Martini.ppm");
    this->vertexShaderFiles = {"sample_application/resources/cube-vert.spv"};
    this->fragmentShaderFiles = {"sample_application/resources/cube-frag.spv"};
}
