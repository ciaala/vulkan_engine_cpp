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
        ResourceModel();

        float *getVertex();

        ResourceModel *setVertex(std::vector<float> &vertex);

        float *getUV();

        ResourceModel *setUV(std::vector<float> &uv);


        ResourceModel *setTextures(std::vector<std::string> &textures);

        vlk::ResourceModel *setVertexShaders(std::vector<std::string> &vertexShaders);

        vlk::ResourceModel *setFragmentShaders(std::vector<std::string> &fragmentShaders);

        std::vector<std::string> &getTextures();

    private:

        std::vector<float> vertex;
        std::vector<float> uv;

        std::vector<std::string> textures;
        std::vector<std::string> vertexShaders;
        std::vector<std::string> fragmentShaders;
    };
}


#endif //VULKAN_ENGINE_CPP_RESOURCEMODEL_HPP
