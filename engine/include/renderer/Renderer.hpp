//
// Created by crypt on 15/07/17.
//

#ifndef VULKAN_ENGINE_CPP_RENDERER_HPP
#define VULKAN_ENGINE_CPP_RENDERER_HPP

#include "VulkanModule.hpp"
#include "XCBModule.hpp"
#include "ShaderModule.hpp"
#include "VulkanDrawableObject.hpp"
// #include "SpacePartitioner.hpp"

namespace vlk {
    class XCBModule;

    class VulkanModule;

    class Renderer {

    private:
        static uint16_t const WINDOW_WIDTH;
        static uint16_t const WINDOW_HEIGHT;
        std::unordered_map<GameObject::SID, VulkanDrawableObject *>
                vulkanDrawables;
        //SpacePartitioner *spacePartitioner = ScenePartitionerFactory.createBSPScenePartitioner();
#if defined(VK_USE_PLATFORM_XCB_KHR)
        XCBModule *xcbModule;
#endif

        VulkanModule *vulkanModule;
        Engine *engine;

    public:
        XCBModule *getXCBModule();

        Renderer(Engine *engine, VulkanModule *vulkanModule, XCBModule *xcbModule);

        void initWindowLibrary();

        void initVulkan();

        void createWindow();

        void initSwapChain();

        void prepare(GameWorld *gameWorld);

        void draw(GameWorld *gameWorld);

        void prepareDrawableObject(vlk::Camera *camera, vlk::GameObject *gameObject);
    };
}

#endif //VULKAN_ENGINE_CPP_RENDERER_HPP
