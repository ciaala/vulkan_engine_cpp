//
// Created by crypt on 25/08/18.
//

#include <iostream>

#include "Engine.hpp"
#include "Application.hpp"

int main(int argc, char** args) {
  vlk::Engine::static_initialization();

  vlk::Engine engine;
  Application application(&engine);
  engine.setApplication(&application);
  engine.init();
  engine.cleanup();
  return 0;
}
