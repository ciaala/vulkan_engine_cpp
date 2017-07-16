//
// Created by crypt on 16/07/17.
//

#ifndef VULKAN_ENGINE_CPP_XCBMODULE_HPP
#define VULKAN_ENGINE_CPP_XCBMODULE_HPP

namespace vlk {
    class Engine;
}
#include <xcb/xcb.h>
#include "Engine.hpp"
namespace vlk {

    class XCBModule {
    private:
        xcb_window_t xcb_window;
        xcb_screen_t *screen;
        xcb_connection_t *connection;
        xcb_intern_atom_reply_t *atom_wm_delete_window;
        bool quit;

        float spin_angle;
        float spin_increment;
        bool pause;

        uint16_t height;
        uint16_t width;
        int curFrame;  unsigned int frameCount;
        vlk::Engine *engine;
    public:
        void createWindow(uint16_t width, uint16_t height);

        void initXCBLibrary();

        void handleXCBEvent(const xcb_generic_event_t *event);

        void runXCB();

    };
}


#endif //VULKAN_ENGINE_CPP_XCBMODULE_HPP
