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
#include "RendererDefinition.hpp"

#include <cstdint>


#include <vulkan/vk_sdk_platform.h>

#include <game/GameObject.hpp>
#include <game/Camera.hpp>
#include "core/linmath.h"

#include "Engine.hpp"
#include "TextureModule.hpp"
#include "core/CommonMacro.hpp"
#include "ShaderModule.hpp"

// Definition used in prepare

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

        void prepare();

        void
        prepareCubeDataBuffers(Camera *camera, GameObject *object);

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

        // Data used inside the prepare phase
        vk::CommandPool cmd_pool;
        vk::CommandPool present_cmd_pool;

        vk::CommandBuffer cmd;  // Buffer for initialization commands
        vk::PipelineLayout pipeline_layout;
        vk::DescriptorSetLayout desc_layout;
        vk::PipelineCache pipelineCache;
        vk::RenderPass render_pass;
        vk::Pipeline pipeline;

        // DISABLED when on switch to GameWorld
        //mat4x4 projection_matrix;
        // mat4x4 view_matrix;

        // DISABLED when on switch to GameObject
        // mat4x4 model_matrix;

        uint32_t swapchainImageCount;
        vk::SwapchainKHR swapchain;
        std::unique_ptr<SwapchainImageResources[]> swapchain_image_resources;

        //TODO Use std::vector
        //static int32_t const texture_count = 1;
        //TODO Use std::vector
        //texture_object textures[texture_count];

        std::vector<texture_object> textures;

        texture_object staging_texture;

        void prepareDescriptorLayout();

        void prepareDescriptorPool();

        void prepareDescriptorSet();

        void prepareFramebuffers();

        void prepareRenderPass();

        //void preparePipeline();

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

        void drawBuildCmd(vk::CommandBuffer buffer, std::vector<vk::CommandBuffer> &subCommands);
        void prepareSubCommandBuffer(const vk::CommandBuffer &commandBuffer, const vk::Viewport *viewport,
                                             const vk::Rect2D *scissor);

        void flushInitCmd();

        TextureModule *textureModule;

        void buildImageOwnershipCmd(uint32_t const &i);

        //static char const *const tex_files[];
        bool use_staging_buffer;


        ShaderModule *shaderModule;
        MemoryModule *memoryModule;
        vk::ShaderModule vert_shader_module;
        vk::ShaderModule frag_shader_module;

        // prepare the draw
    private:
        // float spin_angle;
        void updateDataBuffer(Camera *camera, GameObject *Object);

        // float spin_increment;
        bool pause{false};
    public:
        void resize();

        // Game World Refactor
    public:
        // TODO Move to Texture Module
        vk::FormatProperties textureFormatProperties;
        vk::Format const textureFormat = vk::Format::eR8G8B8A8Unorm;

        void draw(GameWorld *world);

        void prepareTextures();

        void prepareTexture(const char *textureFile, const vk::Format &tex_format);

        void prepareTexture(std::string &basic_string);

        void prepareCamera(vlk::Camera *camera);

        void prepareDescriptors(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList);

        void preparePipeline(std::vector<vk::PipelineShaderStageCreateInfo> &shaderStageInfoList);

        ShaderModule *getShaderModule();
    };
}

#endif //VULKAN_ENGINE_CPP_VULKANMODULE_HPP
