#include <iostream>
#include "Engine.hpp"
#include "SampleApplication.hpp"

int main() {
    vlk::Engine::static_initialization();

    vlk::Engine engine;
    SampleApplication sampleApplication(&engine);
    engine.init();
    engine.cleanup();
    return 0;
}