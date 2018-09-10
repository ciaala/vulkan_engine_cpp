//
// Created by crypt on 24/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
#define VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include "ResourceModel.hpp"
#include "../../src/resource/json.hpp"
using json = nlohmann::json;


namespace vlk {
class ResourceManager {

 private:
  static std::vector<std::string> properties;

 private:

  // TODO MAKE CONSTANT
  std::string loadPath;
  static std::string emptyPath;
 public:
  explicit ResourceManager(const std::string &customPath = emptyPath);

  ResourceModel *loadModel(const std::string &identifier);

  // PRIVATE METHOD
 private:

  ResourceModel *loadJSONModel(const std::string &identifier);

  std::unordered_map<std::string, ResourceModel *> modelCache;
  std::vector<std::string> relocate(json references, std::string string);
};
}

#endif //VULKAN_ENGINE_CPP_RESOURCEMANAGER_HPP
