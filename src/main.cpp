#include <iostream>
#include "Engine.hpp"
#include "SampleApplication.hpp"

int main() {
    std::cout << "VulkanEngine startup." << std::endl;
   // vlk::engine engine;
    std::cout << "Vulkan version: " << vlk::Engine::getVulkanVersion() << std::endl ;
    SampleApplication sampleApplication;
    vlk::Engine engine(&sampleApplication);
    engine.init();
    engine.cleanup();
    return 0;
}