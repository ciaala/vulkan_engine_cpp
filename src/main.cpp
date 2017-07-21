#include <iostream>
#include "Engine.hpp"
#include "SampleApplication.hpp"

int main() {
    std::cout << "VulkanEngine startup." << std::endl;
    // vlk::engine engine;
    std::cout << "Vulkan version: " << vlk::Engine::getVulkanVersion() << std::endl;

    vlk::Engine engine;
    SampleApplication sampleApplication(&engine);
    engine.init();
    engine.cleanup();
    return 0;
}