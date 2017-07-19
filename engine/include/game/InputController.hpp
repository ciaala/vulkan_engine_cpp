//
// Created by crypt on 19/07/17.
//
namespace vlk {
    class InputController;
}

#ifndef VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP
#define VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP


#include <core/InputModule.hpp>

class Application;
namespace vlk {

    class InputController {
    public:
        virtual void keyReleased
                (vlk::Keys key) = 0;
    };

}
#endif //VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP
