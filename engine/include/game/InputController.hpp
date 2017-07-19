//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP
#define VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP


#include <core/InputModule.hpp>

class Application;
namespace vlk {
    enum Keys : uint64_t;
}
class InputController {
public:
    virtual void keyReleased
            (vlk::Keys key) = 0;
};


#endif //VULKAN_ENGINE_CPP_INPUTCONTROLLER_HPP
