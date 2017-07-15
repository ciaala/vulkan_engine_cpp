//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERER_HPP
#define VULKAN_ENGINE_CPP_RENDERER_HPP
#define VK_USE_PLATFORM_XCB_KHR
#include <xcb/xcb.h>
#include <X11/Xutil.h>

namespace vlk {
    class Renderer {

#if defined(VK_USE_PLATFORM_XCB_KHR)
        xcb_window_t xcb_window;
        xcb_screen_t *screen;
        xcb_connection_t *connection;
        xcb_intern_atom_reply_t *atom_wm_delete_window;
#endif
    public:
        Renderer();

        void init_device();

        void initWindowLibrary();
    };
}

#endif //VULKAN_ENGINE_CPP_RENDERER_HPP
