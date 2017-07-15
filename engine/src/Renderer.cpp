//
// Created by crypt on 15/07/17.
//

#include <iostream>
#include "../include/Renderer.hpp"
namespace vlk {
    Renderer::Renderer() {
        this->initWindowLibrary();
        this->init_device();
    }
    void Renderer::initWindowLibrary() {
#if defined(VK_USE_PLATFORM_XCB_KHR)
        const xcb_setup_t *setup;
        xcb_screen_iterator_t iter;
        int scr;

        connection = xcb_connect(nullptr, &scr);
        if (xcb_connection_has_error(connection) > 0) {
            printf(
                "Cannot find a compatible Vulkan installable client driver "
                "(ICD).\nExiting ...\n");
            fflush(stdout);
            exit(1);
        }

        setup = xcb_get_setup(connection);
        iter = xcb_setup_roots_iterator(setup);
        while (scr-- > 0) xcb_screen_next(&iter);

        screen = iter.data;
#else
        std::cout << "Unable to idenfiy window library. Supported {xcb}" << std::endl;
#endif
        }
    void Renderer::init_device() {

    }
}