//
// Created by crypt on 16/07/17.
//

#ifndef VULKAN_ENGINE_CPP_VULKANMODULE_HPP
#define VULKAN_ENGINE_CPP_VULKANMODULE_HPP
// Allow a maximum of two outstanding presentation operations.
#define FRAME_LAG 2

namespace vlk {
    class Engine;
};

#include <cstdint>
#include <xcb/xcb.h>

#define VK_USE_PLATFORM_XCB_KHR

#include "vulkan/vulkan.hpp"
#include "Engine.hpp"

namespace vlk {
    class VulkanModule {
    private:
        Engine *engine;
    private:

        uint32_t instance_extension_count = 0;
        uint32_t instance_layer_count = 0;
        uint32_t validation_layer_count = 0;
        char const *const *instance_validation_layers = nullptr;

        uint32_t enabled_extension_count;
        uint32_t enabled_layer_count;
        char const *extension_names[64];
        char const *enabled_layers[64];

        vk::Instance inst;
        vk::PhysicalDevice gpu;
        vk::Device device;
        vk::Queue graphics_queue;
        vk::Queue present_queue;
        uint32_t graphics_queue_family_index;
        uint32_t present_queue_family_index;
        vk::Semaphore image_acquired_semaphores[FRAME_LAG];
        vk::Semaphore draw_complete_semaphores[FRAME_LAG];
        vk::Semaphore image_ownership_semaphores[FRAME_LAG];
        vk::PhysicalDeviceProperties gpu_props;
        std::unique_ptr<vk::QueueFamilyProperties[]> queue_props;
        vk::PhysicalDeviceMemoryProperties memory_properties;

        uint32_t queue_family_count;

        bool separate_present_queue;

    public:
        VulkanModule(vlk::Engine *engine, bool validate);

        void init();

        void initValidation();

        bool validate;

        vk::Bool32 checkLayers(uint32_t check_count, const char *const *const check_names, uint32_t layer_count,
                               vk::LayerProperties *layers);

        void initSwapChain();

        void initSurface(xcb_connection_t *connection, unsigned int xcb_window);

        vk::SurfaceKHR surface;

        void createDevice();

    private:

        vk::Format format;
        vk::ColorSpaceKHR color_space;
        bool quit;
        uint32_t curFrame;
        uint32_t frame_index;
        vk::Fence fences[FRAME_LAG];
    };
}

#endif //VULKAN_ENGINE_CPP_VULKANMODULE_HPP
