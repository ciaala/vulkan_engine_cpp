//
// Created by crypt on 24/07/17.
//

#include <glog/logging.h>
#include "ResourceManager.hpp"
#include <sys/stat.h>

std::string vlk::ResourceManager::emptyPath = "";

vlk::ResourceManager::ResourceManager(const std::string &customPath) {
    // TODO make Windows ready
    LOG(INFO) << "Resource Manager started!" << std::endl;
    struct stat st{0};

    if (customPath.length() > 0) {
        this->loadPath = customPath;
    } else {
        char currentPath[4096];
        if (getcwd(currentPath, 4096) == currentPath) {
            this->loadPath = std::string(currentPath) + "/resources";
        } else {
            LOG(FATAL) << "Unable to discover current directory" << std::endl;
        }
    }

    if (stat(loadPath.c_str(), &st) == 0 && (st.st_mode & S_IFDIR != 0)) {
        LOG(INFO) << "Loading resources from path: '" << loadPath << "'" << std::endl;
    } else {
        LOG(ERROR) << "Unable to open the directory '" << this->loadPath << "' " << std::endl;
    }
}

vlk::ResourceModel *vlk::ResourceManager::loadModel(std::string &identifier) {
    return new ResourceModel();
}
