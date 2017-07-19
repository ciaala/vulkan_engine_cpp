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
#include <vulkan/vk_sdk_platform.h>
#include <game/GameObject.hpp>
#include "core/linmath.h"

#include "Engine.hpp"
#include "TextureModule.hpp"
#include "core/CommonMacro.hpp"
#include "ShaderModule.hpp"

// Definition used in prepare

struct vkcube_vs_uniform {
    // Must start with MVP
    float mvp[4][4];
    float position[12 * 3][4];
    float color[12 * 3][4];
};


typedef struct {
    vk::Image image;
    vk::CommandBuffer cmd;
    vk::CommandBuffer graphics_to_present_cmd;
    vk::ImageView view;
    vk::Buffer uniform_buffer;
    vk::DeviceMemory uniform_memory;
    vk::Framebuffer framebuffer;
    vk::DescriptorSet descriptor_set;
} SwapchainImageResources;

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

        void prepare(const float *g_vertex_buffer_data, const float *g_uv_buffer_data, GameObject *object);

    private:

        vk::Format format;
        vk::ColorSpaceKHR color_space;
        bool quit;
        uint32_t curFrame;
        uint32_t frame_index;
        vk::Fence fences[FRAME_LAG];


    private:
        void prepareBuffers();

        void prepareDepth();

        void prepareTextures();


        // Data used inside the prepare phase
        vk::CommandPool cmd_pool;
        vk::CommandPool present_cmd_pool;

        vk::CommandBuffer cmd;  // Buffer for initialization commands
        vk::PipelineLayout pipeline_layout;
        vk::DescriptorSetLayout desc_layout;
        vk::PipelineCache pipelineCache;
        vk::RenderPass render_pass;
        vk::Pipeline pipeline;

        mat4x4 projection_matrix;
        mat4x4 view_matrix;
        //mat4x4 model_matrix;

        uint32_t swapchainImageCount;
        vk::SwapchainKHR swapchain;
        std::unique_ptr<SwapchainImageResources[]> swapchain_image_resources;

        static int32_t const texture_count = 1;
        texture_object textures[texture_count];
        texture_object staging_texture;

        void prepareDescriptorLayout();

        void prepareDescriptorPool();

        void prepareDescriptorSet();

        void prepareFramebuffers();

        void prepareRenderPass();

        void preparePipeline();

        struct {
            vk::Format format;
            vk::Image image;
            vk::MemoryAllocateInfo mem_alloc;
            vk::DeviceMemory mem;
            vk::ImageView view;
        } depth;

        uint32_t current_buffer;
        vk::DescriptorPool desc_pool;
        bool prepared{false};
        vk::PresentModeKHR presentMode{vk::PresentModeKHR::eFifo};


        uint32_t width;
        uint32_t height;

        void drawBuildCmd(vk::CommandBuffer buffer);

        void flushInitCmd();

        TextureModule *textureModule;

        void buildImageOwnershipCmd(uint32_t const &i);

        static char const *const tex_files[];
        bool use_staging_buffer;

        vk::ShaderModule prepare_fs();

        vk::ShaderModule prepare_vs();

        ShaderModule *shaderModule;
        MemoryModule *memoryModule;
        vk::ShaderModule vert_shader_module;
        vk::ShaderModule frag_shader_module;

        void
        prepareCubeDataBuffers(const float *g_vertex_buffer_data,
                               const float *g_uv_buffer_data,
                               GameObject *object);

        // prepare the draw
    private:
        // float spin_angle;
        void updateDataBuffer(GameObject *Object);

        // float spin_increment;
        bool pause{false};
    public:
        void resize();

        void draw(GameObject *object);

    };
}

#endif //VULKAN_ENGINE_CPP_VULKANMODULE_HPP
