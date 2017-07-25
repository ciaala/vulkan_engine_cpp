//
// Created by crypt on 25/07/17.
//

#include "game/GameObject.hpp"

std::atomic<vlk::sid_t> vlk::GameObject::GLOBAL_SID{0};

vlk::GameObject::GameObject() {
  this->sid = GLOBAL_SID.fetch_add(1);
  mat4x4_identity(this->modelMatrix);
  this->spinningAngle = 4.0;
  this->vertexBufferData = nullptr;
  this->uvBufferData = nullptr;
}
