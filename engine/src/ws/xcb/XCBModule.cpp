//
// Created by crypt on 16/07/17.
//

#include <cstdint>
#include <unistd.h>
#include "renderer/XCBModule.hpp"
#include "Engine.hpp"
#include <iostream>

#include <xcb/xcb_keysyms.h>
#include <X11/Xutil.h>

void vlk::XCBModule::initXCBLibrary() {
  FLOG(INFO);
  int scr;
  this->connection = xcb_connect(nullptr, &scr);
  if (xcb_connection_has_error(this->connection) > 0) {
    std::cout
        << "Cannot find a compatible Vulkan installable client driver (ICD)." << std::endl
        << "Exiting ..." << std::endl
        << std::flush;
    exit(1);
  }
  const xcb_setup_t *setup = xcb_get_setup(this->connection);
  xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);;

  while (scr-- > 0) xcb_screen_next(&iter);

  this->screen = iter.data;

}

void vlk::XCBModule::createWindow(uint16_t width, uint16_t height) {
  FLOG(INFO);

  this->width = width;
  this->height = height;

  uint32_t value_mask, value_list[32];

  xcb_window = xcb_generate_id(connection);

  value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  value_list[0] = screen->black_pixel;
  value_list[1] = XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_STRUCTURE_NOTIFY;

  xcb_create_window(connection,
                    XCB_COPY_FROM_PARENT,
                    xcb_window, screen->root,
                    0, 0,
                    this->width, this->height,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    screen->root_visual,
                    value_mask,
                    value_list);
  /* Change Window Name */
  auto name = engine->getApplication()->getName();
  xcb_change_property(connection,
                      XCB_PROP_MODE_REPLACE,
                      xcb_window,
                      XCB_ATOM_WM_NAME,
                      XCB_ATOM_STRING,
                      8,
                      (uint32_t) name.length(),
                      name.c_str());
  /* Magic code that will send notification when window is destroyed */
  xcb_intern_atom_cookie_t cookie = xcb_intern_atom(connection, 1, 12, "WM_PROTOCOLS");
  xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(connection, cookie, 0);

  xcb_intern_atom_cookie_t cookie2 = xcb_intern_atom(connection, 0, 16, "WM_DELETE_WINDOW");
  atom_wm_delete_window = xcb_intern_atom_reply(connection, cookie2, 0);

  xcb_change_property(connection,
                      XCB_PROP_MODE_REPLACE,
                      xcb_window,
                      (*reply).atom, 4, 32, 1,
                      &(*atom_wm_delete_window).atom);

  free(reply);

  xcb_map_window(connection, xcb_window);
  xcb_flush(connection);
  // Force the x/y coordinates to 100,100 results are identical in
  // consecutive
  // runs
  const uint32_t coords[] = {100, 100};
  xcb_configure_window(connection, xcb_window, XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y, coords);
}

/* wrapper to get xcb keysymbol from keycode */
static xcb_keysym_t xcb_get_keysym(xcb_connection_t *connection, xcb_keycode_t keycode) {
  xcb_key_symbols_t *keysyms;
  xcb_keysym_t keysym;

  if (!(keysyms = xcb_key_symbols_alloc(connection))) { return 0; }
  keysym = xcb_key_symbols_get_keysym(keysyms, keycode, 0);
  xcb_key_symbols_free(keysyms);

  return keysym;
}

void vlk::XCBModule::handleXCBEvent(const xcb_generic_event_t *event) {
  int32_t event_code = event->response_type & 0x7f;
  std::stringstream eventDetail;

  switch (event_code) {
    case XCB_EXPOSE:
      // TODO: Resize window
      eventDetail << "Event XCB_EXPOSE";
      break;
    case XCB_CLIENT_MESSAGE: {
      eventDetail <<  "Event XCB_CLIENT_MESSAGE";
      auto message = (const xcb_client_message_event_t *) event;
      if (message->data.data32[0] == (*atom_wm_delete_window).atom) {
        quit = true;
      }
      break;
    }
    case XCB_KEY_RELEASE: {
      auto *key = (const xcb_key_release_event_t *) event;
      xcb_keysym_t sym = xcb_get_keysym(connection, key->detail);
      eventDetail << "Received KEY_RELEASE" << sym << std::endl;

      switch (sym) {
        case XK_Escape:
          //quit = true;
          this->inputModule->sendKeyReleased(Keys::ESCAPE);
          break;
        case XK_Left:
          //spin_angle -= spin_increment;
          this->inputModule->sendKeyReleased(Keys::ARROW_LEFT);
          break;
        case XK_Right:this->inputModule->sendKeyReleased(Keys::ARROW_RIGHT);
          //spin_angle += spin_increment;
          break;
        case XK_space:this->inputModule->sendKeyReleased(Keys::SPACE_BAR);
          //isPaused = !isPaused;
          break;
        default:
          std::cerr << "Event XCB_KEY_RELEASE not mapped (code: " << (char) key->detail << ", sym: " << sym
                    << ")" << std::endl;
      }
      break;
    }
    case XCB_CONFIGURE_NOTIFY: {
      eventDetail << "Event XCB_CONFIGURE_NOTIFY" << std::endl;

      const xcb_configure_notify_event_t *cfg = (const xcb_configure_notify_event_t *) event;
      if ((width != cfg->width) || (height != cfg->height)) {
        width = cfg->width;
        height = cfg->height;
        engine->resize(width, height);
      }
      break;
    }
    case XCB_REPARENT_NOTIFY : {
      eventDetail <<  "Event XCB_REPARENT_NOTIFY" << std::endl;
      break;
    }
    case XCB_MAP_REQUEST : {
      eventDetail <<  "Event XCB_MAP_REQUEST" << std::endl;
      break;
    }

    case XCB_MAP_NOTIFY: {
      eventDetail <<  "Event XCB_MAP_NOTIFY" << std::endl;
      break;
    }
    default: {
      eventDetail <<  "Not handled event with code: " << event_code << std::endl;
      break;
    }
  }
  FLOG(INFO) << eventDetail.str();
}

void vlk::XCBModule::runXCB() {
  FLOG(INFO);
  xcb_flush(connection);

  while (!quit) {
    xcb_generic_event_t *event;

    if (isPaused) {
      event = xcb_wait_for_event(connection);
    } else {
      event = xcb_poll_for_event(connection);
    }
    while (event) {
      this->handleXCBEvent(event);
      free(event);
      event = xcb_poll_for_event(connection);
    }
if (engine->isReady()) {
    this->engine->draw();
    curFrame++;
    if (frameCount != UINT32_MAX && curFrame == frameCount) {
      quit = true;
    }}
  }
}

vlk::XCBModule::XCBModule(vlk::Engine *engine) : frameCount(0), curFrame(0) {
  this->engine = engine;
  this->inputModule = engine->getInputModule();

}

xcb_connection_t *vlk::XCBModule::getConnection() {
  return this->connection;
}

xcb_window_t vlk::XCBModule::getWindow() {
  return this->xcb_window;
}

