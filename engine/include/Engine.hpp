//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_ENGINE_H
#define VULKAN_ENGINE_CPP_ENGINE_H

#include <vulkan/vulkan.hpp>
#include <string>
#include "Application.hpp"
#include "Renderer.hpp"
#include "XCBModule.hpp"
#include "VulkanModule.hpp"

namespace vlk {
    class Renderer;

    class XCBModule;

    class VulkanModule;

    class Engine {
    private:
        // int argc;
        // char** argv;
        bool DEBUG_LEVEL = true;
        Application *application;
        Renderer *renderer;
#if defined(VK_USE_PLATFORM_XCB_KHR)
        XCBModule *xcbModule;
#endif

        VulkanModule *vulkanModule;
    public:
        //engine(int argc = 0, char** argv = nullptr);

        Engine(Application *application);

        void setupModules();

        void init();

        void draw();

        void resize();

        void prepare();

        void cleanup();

        Application *getApplication() { return this->application; }

    public:
        // ---- STATIC

        static std::string getVulkanVersion();

        static std::string getName();
    };
}


#endif //VULKAN_ENGINE_CPP_ENGINE_H
