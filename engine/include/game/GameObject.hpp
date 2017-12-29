//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
#define VULKAN_ENGINE_CPP_GAMEOBJECT_HPP

#include <core/linmath.h>
#include <vector>
#include <string>
#include <atomic>
#include <vulkan/vulkan.hpp>

namespace vlk {
class GameObject {

 public:
  typedef unsigned long long SID;

 private:
  static std::atomic<SID> GLOBAL_SID;
  SID sid;
 public:

  GameObject();

  SID getSid() { return this->sid; }

  mat4x4 &getModelMatrix() {
    return this->modelMatrix;
  };

  float getSpinningAngle() {
    return this->spinningAngle;
  }

  std::vector<std::string> getTextureFiles() {
    return textureFiles;
  }

  float *getVertexBufferData() {
    return vertexBufferData;
  }

  float *getUVBufferData() {
    return uvBufferData;
  }

  std::vector<std::string> &getVertexShaderFiles() {
    return vertexShaderFiles;
  }

  std::vector<std::string> &getFragmentShaderFiles() {
    return fragmentShaderFiles;
  }

 protected:
  float spinningAngle;
  mat4x4 modelMatrix;
  std::vector<std::string> textureFiles;
  float *vertexBufferData;
  float *uvBufferData;

  std::vector<std::string> vertexShaderFiles;
  std::vector<std::string> fragmentShaderFiles;
  vk::Pipeline pipeline;
};

}

#endif //VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
