//
// Created by crypt on 25/08/18.
//

#ifndef VULKAN_ENGINE_CPP_UTILITY_HPP
#define VULKAN_ENGINE_CPP_UTILITY_HPP
#include <filesystem>
//using filesystem = std::filesystem;

class Utility {
 public:
  static std::filesystem::path getAbsolutePath(std::string filename){
    auto path = std::filesystem::path(filename);
    return std::filesystem::absolute(path);
  }

};

#endif //VULKAN_ENGINE_CPP_UTILITY_HPP
