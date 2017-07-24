//
// Created by crypt on 24/07/17.
//

#include <glog/logging.h>
#include "resource/ResourceManager.hpp"
#include <sys/stat.h>
#include "json.hpp"
#include <fstream>

std::string vlk::ResourceManager::emptyPath = "";
using json = nlohmann::json;

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

vlk::ResourceModel *vlk::ResourceManager::loadModel(const std::string &identifier) {
    std::ifstream file(this->loadPath + "/" + identifier);
    std::stringstream ss;
    if (file.is_open()) {

        std::string line;
        json p;
        while (!file.eof()) {
            getline(file, line);
            size_t left_start = line.find_first_not_of(' ');
            if (left_start >= line.length() - 1 ||
                !(line[left_start] == '/' && line[left_start + 1] == '/')) {
                ss << line;
            } else {
                // it is a comment
            }
        }
        //std::cout << ss.str() << std::endl;
        file.close();
        p << ss;
        //std::cout << p << std::endl;
        if (p.at("vertex").is_array() &&
            p.at("uv").is_array()) {
            std::vector<float> vertex = p["vertex"];
            std::vector<float> uv = p["uv"];
            return new ResourceModel(vertex, uv);
        }
    }
    LOG(ERROR) << "Unable to open the file" << std::endl;
    // TODO Discuss Engine API behaviour on exception
    return nullptr;
}
