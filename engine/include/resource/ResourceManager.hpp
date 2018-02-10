//
// Created by crypt on 24/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
#define VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include "ResourceModel.hpp"

namespace vlk {
    class ResourceManager {
    private:
        // TODO MAKE CONSTANT
        std::string loadPath;
        static std::string emptyPath;
    public:
        explicit ResourceManager(const std::string &customPath = emptyPath);

        ResourceModel *loadModel(const std::string &identifier);

    private:

        ResourceModel *loadJSONModel(const std::string &identifier);

        std::unordered_map<std::string, ResourceModel *> modelCache;
    };
}


#endif //VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
