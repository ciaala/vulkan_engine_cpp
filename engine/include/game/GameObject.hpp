//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
#define VULKAN_ENGINE_CPP_GAMEOBJECT_HPP

#include <core/linmath.h>
#include <vector>
#include <string>

namespace vlk {
    class GameObject {

    public:
        GameObject() {
            mat4x4_identity(this->modelMatrix);
            this->spinningAngle = 4.0;
            this->vertexBufferData = nullptr;
            this->uvBufferData = nullptr;
        }

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
    };

}

#endif //VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
