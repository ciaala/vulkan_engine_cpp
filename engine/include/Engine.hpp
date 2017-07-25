//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_ENGINE_H
#define VULKAN_ENGINE_CPP_ENGINE_H

#include <string>
#include "game/Application.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/XCBModule.hpp"
#include "renderer/VulkanModule.hpp"
#include "core/InputModule.hpp"
#include "../src/audio/AudioModule.hpp"
#include "resource/ResourceManager.hpp"

namespace vlk {
    class Renderer;

    class XCBModule;

    class VulkanModule;

    class InputModule;


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
        InputModule *inputModule;
        VulkanModule *vulkanModule;
        AudioModule *audioModule;

    public:
        //engine(int argc = 0, char** argv = nullptr);

        explicit Engine();

        void setupModules();

        void init();

        void draw();

        void resize(uint32_t width, u_int32_t height);

        void prepare();

        void cleanup();

        Application *getApplication() { return this->application; }

        InputModule *getInputModule() { return this->inputModule; };

        vlk::ResourceManager *getResourceManager();

    public:
        // ---- STATIC
        static std::string getVulkanVersion();

        static std::string getName();

        void queue_audio_effect(GameObject *gameObject, const std::string audioFilename);

        void setApplication(Application *application) { this->application = application; }

    private:

        ResourceManager *resourceManager;
    };
}


#endif //VULKAN_ENGINE_CPP_ENGINE_H
