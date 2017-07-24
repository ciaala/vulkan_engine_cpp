//
// Created by crypt on 24/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
#define VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP

#include <string>
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


    };
}


#endif //VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
