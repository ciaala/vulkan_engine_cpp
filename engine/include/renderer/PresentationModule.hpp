//
// Created by Francesco Fiduccia on 06/03/2018.
//

#ifndef VULKAN_ENGINE_CPP_PRESENTATIONMODULE_HPP
#define VULKAN_ENGINE_CPP_PRESENTATIONMODULE_HPP

#include "core/CommonMacro.hpp"

namespace vlk {
    class PresentationModule {
        virtual void createWindow(uint16_t width, uint16_t height) = 0;

    };
}


#endif //VULKAN_ENGINE_CPP_PRESENTATIONMODULE_HPP
