//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_ENGINE_H
#define VULKAN_ENGINE_CPP_ENGINE_H

#include <vulkan/vulkan.hpp>
#include <string>
#include "Application.hpp"
#include "Renderer.hpp"

namespace vlk {
    class Renderer;

    class Engine {
    private:
        // int argc;
        // char** argv;
        bool DEBUG_LEVEL = true;
        Application *application;
        Renderer *renderer;
    public:
        //engine(int argc = 0, char** argv = nullptr);

        Engine(Application *application);

        void setup();

        void init();

        void draw();

        void resize();

        Application* getApplication() { return this->application; }

    public:
        // ---- STATIC

        static std::string getVulkanVersion();

        static std::string getName();
    };
}


#endif //VULKAN_ENGINE_CPP_ENGINE_H
