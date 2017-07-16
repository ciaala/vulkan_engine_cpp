//
// Created by crypt on 16/07/17.
//

#include <cstdint>
#include "../../include/XCBModule.hpp"
#include "../../include/Engine.hpp"
#include <iostream>

void vlk::XCBModule::initXCBLibrary() {
    const xcb_setup_t *setup;
    xcb_screen_iterator_t iter;
    int scr;

    this->connection = xcb_connect(nullptr, &scr);
    if (xcb_connection_has_error(this->connection) > 0) {
        std::cout
                << "Cannot find a compatible Vulkan installable client driver (ICD)." << std::endl
                << "Exiting ..." << std::endl
                << std::flush;
        exit(1);
    }

    setup = xcb_get_setup(this->connection);
    iter = xcb_setup_roots_iterator(setup);
    while (scr-- > 0) xcb_screen_next(&iter);

    this->screen = iter.data;
}

void vlk::XCBModule::createWindow(uint16_t width, uint16_t height) {
    uint32_t value_mask, value_list[32];

    xcb_window = xcb_generate_id(connection);

    value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    value_list[0] = screen->black_pixel;
    value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    xcb_create_window(connection, XCB_COPY_FROM_PARENT, xcb_window, screen->root, 0, 0, this->width, this->height, 0,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, value_mask, value_list);

    /* Magic code that will send notification when window is destroyed */
    xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
    xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, 0);

    xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
    atom_wm_delete_window = xcb_intern_atom_reply(connection, cookie2, 0);

    xcb_change_property(connection, XCB_PROP_MODE_REPLACE, xcb_window, (*reply).atom, 4, 32, 1,
                        &(*atom_wm_delete_window).atom);

    free(reply);

    xcb_map_window(connection, xcb_window);

    // Force the x/y coordinates to 100,100 results are identical in
    // consecutive
    // runs
    const uint32_t coords[] = {100, 100};
    xcb_configure_window(connection, xcb_window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
}


void vlk::XCBModule::handleXCBEvent(const xcb_generic_event_t *event) {
    uint8_t event_code = event->response_type & 0x7f;
    switch (event_code) {
        case XCB_EXPOSE:
            // TODO: Resize window
            break;
        case XCB_CLIENT_MESSAGE:
            if ((*(xcb_client_message_event_t *)event).data.data32[0] == (*atom_wm_delete_window).atom) {
                quit = true;
            }
            break;
        case XCB_KEY_RELEASE: {
            const xcb_key_release_event_t *key = (const xcb_key_release_event_t *)event;

            switch (key->detail) {
                case 0x9:  // Escape
                    quit = true;
                    break;
                case 0x71:  // left arrow key
                    spin_angle -= spin_increment;
                    break;
                case 0x72:  // right arrow key
                    spin_angle += spin_increment;
                    break;
                case 0x41:  // space bar
                    pause = !pause;
                    break;
            }
        } break;
        case XCB_CONFIGURE_NOTIFY: {
            const xcb_configure_notify_event_t *cfg = (const xcb_configure_notify_event_t *)event;
            if ((width != cfg->width) || (height != cfg->height)) {
                width = cfg->width;
                height = cfg->height;
                engine->resize();
            }
        } break;
        default:
            break;
    }
}

void vlk::XCBModule::runXCB() {
    xcb_flush(connection);

    while (!quit) {
        xcb_generic_event_t *event;

        if (pause) {
            event = xcb_wait_for_event(connection);
        } else {
            event = xcb_poll_for_event(connection);
        }
        while (event) {
            this->handleXCBEvent(event);
            free(event);
            event = xcb_poll_for_event(connection);
        }

        this->engine->draw();
        curFrame++;
        if (frameCount != UINT32_MAX && curFrame == frameCount) {
            quit = true;
        }
    }
}
