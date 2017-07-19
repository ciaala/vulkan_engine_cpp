//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_CAMERA_HPP
#define VULKAN_ENGINE_CPP_CAMERA_HPP


#include <core/linmath.h>

namespace vlk {
    class Camera {
    protected:
        mat4x4 projectionMatrix;
        mat4x4 viewMatrix;
        vec3 eye = {0.0f, 3.0f, 5.0f};
        vec3 origin = {0, 0, 0};
        vec3 up = {0.0f, 1.0f, 0.0};
    public:
        mat4x4 &getProjectionMatrix() {
            return projectionMatrix;
        }

        mat4x4 &getViewMatrix() {
            return viewMatrix;
        }

        vec3 &getEye() {
            return eye;
        }

        vec3 &getOrigin() {
            return origin;
        }

        vec3 &getUp() {
            return up;
        }
    };
}

#endif //VULKAN_ENGINE_CPP_CAMERA_HPP
