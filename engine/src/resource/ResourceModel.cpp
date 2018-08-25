//
// Created by crypt on 24/07/17.
//

#include <vector>
#include <string>
#include "resource/ResourceModel.hpp"

float *vlk::ResourceModel::getVertex() {
    return this->vertex.data();
}

float *vlk::ResourceModel::getUV() {
    return uv.data();
}

vlk::ResourceModel *vlk::ResourceModel::setVertex(
        std::vector<float> &vertex) {
    this->vertex = vertex;
    return this;
}

vlk::ResourceModel *vlk::ResourceModel::setUV(
        std::vector<float> &uv) {
    this->uv = uv;

    return this;
}

vlk::ResourceModel *vlk::ResourceModel::setTextures(std::vector<std::string> &textures) {
    this->textures = textures;
    return this;
}

vlk::ResourceModel *vlk::ResourceModel::setVertexShaders(std::vector<std::string> &vertexShaders) {
    this->vertexShaders = vertexShaders;
    return this;
}

vlk::ResourceModel *vlk::ResourceModel::setFragmentShaders(std::vector<std::string> &fragmentShaders) {
    this->fragmentShaders = fragmentShaders;
    return this;
}

std::vector<std::string> &vlk::ResourceModel::getTextures() {
    return this->textures;
}

std::vector<std::string> &vlk::ResourceModel::getVertexShaders() {
    return this->vertexShaders;
}

std::vector<std::string> &vlk::ResourceModel::getFragmentShaders() {
    return this->fragmentShaders;
}
