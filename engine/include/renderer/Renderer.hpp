//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERER_HPP
#define VULKAN_ENGINE_CPP_RENDERER_HPP
#define VK_USE_PLATFORM_XCB_KHR

#include <X11/Xutil.h>
#include "VulkanModule.hpp"
#include "XCBModule.hpp"

namespace vlk {
    class XCBModule;

    class VulkanModule;

    class Renderer {

    private:
        static uint16_t const WINDOW_WIDTH;
        static uint16_t const WINDOW_HEIGHT;

#if defined(VK_USE_PLATFORM_XCB_KHR)
        XCBModule *xcbModule;
#endif

        VulkanModule *vulkanModule;
        Engine *engine;

        void prepareObject(GameObject *gameObject);

    public:
        XCBModule *getXCBModule();

        Renderer(Engine *engine, VulkanModule *vulkanModule, XCBModule *xcbModule);


        void initWindowLibrary();

        void initVulkan();

        void createWindow();

        void initSwapChain();

        void prepare(GameWorld *gameWorld);

        void draw(GameWorld *gameWorld);

        void
    prepareShaders(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfo, std::vector<vk::ShaderModule> &vertexes,
                   std::vector<vk::ShaderModule> &fragments);
    };
}

#endif //VULKAN_ENGINE_CPP_RENDERER_HPP
