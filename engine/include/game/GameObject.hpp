//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
#define VULKAN_ENGINE_CPP_GAMEOBJECT_HPP

#include <core/linmath.h>

namespace vlk {
    class GameObject {

    public:
        GameObject() {
            mat4x4_identity(mat4x4);
        }
        mat4x4 &getModelMatrix() {
            return this->mat4x4;
        };

        float getSpinningAngle() {
            return this->spinningAngle;
        }

    protected:
        float spinningAngle;
        mat4x4 mat4x4;
    };

}

#endif //VULKAN_ENGINE_CPP_GAMEOBJECT_HPP
