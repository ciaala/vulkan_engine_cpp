//
// Created by crypt on 24/07/17.
//
namespace vlk {
    class ResourceModel;
}
#ifndef VULKAN_ENGINE_CPP_RESOURCEMODEL_HPP
#define VULKAN_ENGINE_CPP_RESOURCEMODEL_HPP

namespace vlk {
    class ResourceModel {

    public:
        ResourceModel(const std::vector<float> &vertex, const std::vector<float> &uv);

        float *getVertex();

        float *getUV();

    private:

        std::vector<float> vertex;
        std::vector<float> uv;
    };
}


#endif //VULKAN_ENGINE_CPP_RESOURCEMODEL_HPP
