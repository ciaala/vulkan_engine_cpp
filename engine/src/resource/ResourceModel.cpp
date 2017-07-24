//
// Created by crypt on 24/07/17.
//

#include <vector>
#include "resource/ResourceModel.hpp"

float *vlk::ResourceModel::getVertex() {
    return this->vertex.data();
}

float *vlk::ResourceModel::getUV() {
    return uv.data();
}

vlk::ResourceModel::ResourceModel(const std::vector<float> &vertex, const std::vector<float> &uv) {
    // TODO analyse performance
    this->vertex = vertex;
    this->uv = uv;

}
