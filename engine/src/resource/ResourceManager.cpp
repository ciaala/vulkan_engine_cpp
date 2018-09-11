//
// Created by crypt on 24/07/17.
//

#include "core/CommonMacro.hpp"
#include "resource/ResourceManager.hpp"
#include <sys/stat.h>
#include "json.hpp"
#include "../utility/Utility.hpp"
#include <fstream>

using json = nlohmann::json;
namespace vlk {

// CONSTANTS
static const std::string TEXTURE_IMAGES_PROPERTY = "textures";
static const std::string VERTEX_SHADERS_PROPERTY = "vertexShaders";
static const std::string FRAGMENT_SHADERS_PROPERTY = "fragmentShaders";
static const char *const VERTEX_PROPERTY = "vertex";
static const char *const TEXTURE_PROPERTY = "uv";

const std::string ResourceManager::emptyPath = "";

std::vector<std::string> const ResourceManager::properties = {VERTEX_PROPERTY,
     TEXTURE_PROPERTY,
     TEXTURE_IMAGES_PROPERTY,
     VERTEX_SHADERS_PROPERTY,
     FRAGMENT_SHADERS_PROPERTY};

std::vector<std::string>
relocateResource(const std::string loadPath, const json &model, const std::string &property) {
  std::vector<std::string> resources;
  for (auto &element: model[property]) {
    resources.emplace_back(loadPath + std::filesystem::path::preferred_separator + element.get<std::string>());
  }
  return resources;
}

ResourceManager::ResourceManager(const std::string &customPath) {
  // TODO make Windows ready

  struct stat st{0};

  if (customPath.length() > 0) {
    this->loadPath = customPath;
  } else {
    FLOG(INFO) << "using default resource locations" << std::endl;
    char currentPath[4096];

    if (getcwd(currentPath, 4096) == currentPath) {
      this->loadPath = std::string(currentPath) + "/resources";
    } else {
      FLOG(FATAL) << "Unable to discover current directory" << std::endl;
    }
  }

  if (stat(loadPath.c_str(), &st) == 0 && ((st.st_mode & S_IFDIR) != 0)) {
    FLOG(INFO) << "Loading resources from path: '" << Utility::getAbsolutePath(this->loadPath) << "'" << std::endl;
  } else {

    FLOG(ERROR) << "Unable to open the directory " << Utility::getAbsolutePath(this->loadPath) << std::endl;
  }
}

bool containsAllPropertiesTypeArray(json &json, std::vector<std::string> &properties) {
  for (auto &&property : properties) {
    if (!json.at(property).is_array()) {
      return false;
    }
  }
  return true;
}

std::string stripJSONComment(std::string &line) {
  size_t comment_start = line.find_first_of('/');
  if (comment_start < (line.length() - 1) && line[comment_start + 1] == '/') {
    LOG(INFO) << "Stripped " << line << " -> " << line.substr(0, comment_start) << std::endl;
    return line.substr(0, comment_start);
  }
  return line;
}

vlk::ResourceModel *vlk::ResourceManager::loadJSONModel(const std::string &identifier) {
  FLOG(INFO) << "Loading json: " << identifier;

  std::string fullname = this->loadPath + "/" + identifier;
  std::ifstream file(fullname);
  std::stringstream ss;
  if (file.is_open()) {

    std::string line;
    json p;
    while (!file.eof()) {
      getline(file, line);
      ss << stripJSONComment(line);
    }
    //std::cout << ss.str() << std::endl;
    file.close();
    p << ss;
    //std::cout << p << std::endl;

    if (containsAllPropertiesTypeArray(p, properties)) {
      std::vector<float> vertex = p[VERTEX_PROPERTY];
      std::vector<float> uv = p[TEXTURE_PROPERTY];
      std::vector<std::string> textures = relocateResource(this->loadPath, p, TEXTURE_IMAGES_PROPERTY);
      std::vector<std::string> vertexShaders = relocateResource(this->loadPath, p, VERTEX_SHADERS_PROPERTY);
      std::vector<std::string> fragmentShaders = relocateResource(this->loadPath, p, FRAGMENT_SHADERS_PROPERTY);

      return (new ResourceModel())->setVertex(vertex)
          ->setUV(uv)
          ->setTextures(textures)
          ->setVertexShaders(vertexShaders)
          ->setFragmentShaders(fragmentShaders);

    } else {
      LOG(ERROR) << "Check propertie: at least one is missing." << std::endl;
    }
  }
  LOG(ERROR) << "Unable to open the file: " << Utility::getAbsolutePath(fullname) << std::endl;
  // TODO Discuss Engine API behaviour on exception
  return nullptr;
}

vlk::ResourceModel *vlk::ResourceManager::loadModel(const std::string &identifier) {
  FLOG(INFO) << "Loading model: " << identifier;
  if (modelCache.count(identifier) == 0) {

    if (identifier.rfind(".json") == (identifier.length() - 5)) {
      modelCache[identifier] = loadJSONModel(identifier);
    }
  }
  return modelCache[identifier];
}

}
