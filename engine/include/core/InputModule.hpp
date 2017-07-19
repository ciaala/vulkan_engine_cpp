//
// Created by crypt on 19/07/17.
//

#ifndef VULKAN_ENGINE_CPP_INPUTMODULE_HPP
#define VULKAN_ENGINE_CPP_INPUTMODULE_HPP

#include <unordered_map>
#include "../Engine.hpp"

namespace vlk {
    enum Keys : uint64_t {
        ARROW_LEFT = 1,
        ARROW_RIGHT,
        SPACE_BAR,
        ESCAPE
    };

    class Engine;

    class InputModule {

    public:
        InputModule(Engine *engine);

        void sendKeyReleased(Keys key);

        static const std::unordered_map<Keys, std::string> keyToString;
    private:

        Engine *engine;
    };
}

#endif //VULKAN_ENGINE_CPP_INPUTMODULE_HPP
