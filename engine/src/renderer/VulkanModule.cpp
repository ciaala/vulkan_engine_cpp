//
// Created by crypt on 16/07/17.
//

#include "../../include/VulkanModule.hpp"
#include "../../include/Engine.hpp"

#define VERIFY(x) assert(x)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#define ERR_EXIT(err_msg, err_class) \
    do {                             \
        printf("%s\n", err_msg);     \
        fflush(stdout);              \
        exit(1);                     \
    } while (0)

vlk::VulkanModule::VulkanModule(Engine *engine, bool validate) {
    this->engine = engine;
    this->validate = validate;
    enabled_extension_count = 0;
    enabled_layer_count = 0;

}
void vlk::VulkanModule::initValidation(){
    char const *const instance_validation_layers_alt1[] = {"VK_LAYER_LUNARG_standard_validation"};

    char const *const instance_validation_layers_alt2[] = {
            "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation", "VK_LAYER_LUNARG_object_tracker",
            "VK_LAYER_LUNARG_core_validation", "VK_LAYER_GOOGLE_unique_objects"};
    vk::Bool32 validation_found = VK_FALSE;
    if (this->validate) {
        auto result = vk::enumerateInstanceLayerProperties(&instance_layer_count, nullptr);
        VERIFY(result == vk::Result::eSuccess);

        instance_validation_layers = instance_validation_layers_alt1;
        if (instance_layer_count > 0) {
            std::unique_ptr<vk::LayerProperties[]> instance_layers(new vk::LayerProperties[instance_layer_count]);
            result = vk::enumerateInstanceLayerProperties(&instance_layer_count, instance_layers.get());
            VERIFY(result == vk::Result::eSuccess);

            validation_found = this->checkLayers(ARRAY_SIZE(instance_validation_layers_alt1), instance_validation_layers,
                                            instance_layer_count, instance_layers.get());
            if (validation_found) {
                enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt1);
                enabled_layers[0] = "VK_LAYER_LUNARG_standard_validation";
                validation_layer_count = 1;
            } else {
                // use alternative set of validation layers
                instance_validation_layers = instance_validation_layers_alt2;
                enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt2);
                validation_found = this->checkLayers(ARRAY_SIZE(instance_validation_layers_alt2), instance_validation_layers,
                                                instance_layer_count, instance_layers.get());
                validation_layer_count = ARRAY_SIZE(instance_validation_layers_alt2);
                for (uint32_t i = 0; i < validation_layer_count; i++) {
                    enabled_layers[i] = instance_validation_layers[i];
                }
            }
        }

        if (!validation_found) {
            ERR_EXIT(
                    "vkEnumerateInstanceLayerProperties failed to find "
                            "required validation layer.\n\n"
                            "Please look at the Getting Started guide for "
                            "additional information.\n",
                    "vkCreateInstance Failure");
        }
    }
}
void vlk::VulkanModule::init() {

        uint32_t instance_extension_count = 0;
        uint32_t instance_layer_count = 0;
        uint32_t validation_layer_count = 0;
        char const *const *instance_validation_layers = nullptr;
        enabled_extension_count = 0;
        enabled_layer_count = 0;

        char const *const instance_validation_layers_alt1[] = {"VK_LAYER_LUNARG_standard_validation"};

        char const *const instance_validation_layers_alt2[] = {
                "VK_LAYER_GOOGLE_threading", "VK_LAYER_LUNARG_parameter_validation", "VK_LAYER_LUNARG_object_tracker",
                "VK_LAYER_LUNARG_core_validation", "VK_LAYER_GOOGLE_unique_objects"};

        // Look for validation layers
       this->initValidation();

        /* Look for instance extensions */
        vk::Bool32 surfaceExtFound = VK_FALSE;
        vk::Bool32 platformSurfaceExtFound = VK_FALSE;
        memset(extension_names, 0, sizeof(extension_names));

        auto result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
        VERIFY(result == vk::Result::eSuccess);

        if (instance_extension_count > 0) {
            std::unique_ptr<vk::ExtensionProperties[]> instance_extensions(new vk::ExtensionProperties[instance_extension_count]);
            result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, instance_extensions.get());
            VERIFY(result == vk::Result::eSuccess);

            for (uint32_t i = 0; i < instance_extension_count; i++) {
                if (!strcmp(VK_KHR_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    surfaceExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_SURFACE_EXTENSION_NAME;
                }
#if defined(VK_USE_PLATFORM_WIN32_KHR)
                if (!strcmp(VK_KHR_WIN32_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    platformSurfaceExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_WIN32_SURFACE_EXTENSION_NAME;
                }
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
                if (!strcmp(VK_KHR_XLIB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    platformSurfaceExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_XLIB_SURFACE_EXTENSION_NAME;
                }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
                if (!strcmp(VK_KHR_XCB_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    platformSurfaceExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
                }
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
                if (!strcmp(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME, instance_extensions[i].extensionName)) {
                    platformSurfaceExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME;
                }
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
                if (!strcmp(VK_KHR_DISPLAY_EXTENSION_NAME,
                            instance_extensions[i].extensionName)) {
                    platformSurfaceExtFound = 1;
                    extension_names[enabled_extension_count++] =
                        VK_KHR_DISPLAY_EXTENSION_NAME;
                }

#endif
                assert(enabled_extension_count < 64);
            }
        }

        if (!surfaceExtFound) {
            ERR_EXIT(
                    "vkEnumerateInstanceExtensionProperties failed to find "
                            "the " VK_KHR_SURFACE_EXTENSION_NAME
            " extension.\n\n"
                    "Do you have a compatible Vulkan installable client "
                    "driver (ICD) installed?\n"
                    "Please look at the Getting Started guide for additional "
                    "information.\n",
                    "vkCreateInstance Failure");
        }

        if (!platformSurfaceExtFound) {
#if defined(VK_USE_PLATFORM_WIN32_KHR)
            ERR_EXIT(
                "vkEnumerateInstanceExtensionProperties failed to find "
                "the " VK_KHR_WIN32_SURFACE_EXTENSION_NAME
                " extension.\n\n"
                "Do you have a compatible Vulkan installable client "
                "driver (ICD) installed?\n"
                "Please look at the Getting Started guide for additional "
                "information.\n",
                "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_XCB_KHR)
            ERR_EXIT(
                "vkEnumerateInstanceExtensionProperties failed to find "
                "the " VK_KHR_XCB_SURFACE_EXTENSION_NAME
                " extension.\n\n"
                "Do you have a compatible Vulkan installable client "
                "driver (ICD) installed?\n"
                "Please look at the Getting Started guide for additional "
                "information.\n",
                "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
            ERR_EXIT(
                "vkEnumerateInstanceExtensionProperties failed to find "
                "the " VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME
                " extension.\n\n"
                "Do you have a compatible Vulkan installable client "
                "driver (ICD) installed?\n"
                "Please look at the Getting Started guide for additional "
                "information.\n",
                "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
            ERR_EXIT(
                "vkEnumerateInstanceExtensionProperties failed to find "
                "the " VK_KHR_XLIB_SURFACE_EXTENSION_NAME
                " extension.\n\n"
                "Do you have a compatible Vulkan installable client "
                "driver (ICD) installed?\n"
                "Please look at the Getting Started guide for additional "
                "information.\n",
                "vkCreateInstance Failure");
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
            ERR_EXIT("vkEnumerateInstanceExtensionProperties failed to find "
                     "the " VK_KHR_DISPLAY_EXTENSION_NAME " extension.\n\n"
                     "Do you have a compatible Vulkan installable client "
                     "driver (ICD) installed?\n"
                     "Please look at the Getting Started guide for additional "
                     "information.\n",
                     "vkCreateInstance Failure");
#endif
        }
        auto const app = vk::ApplicationInfo()
                .setPApplicationName(engine->getApplication()->getName().c_str())
                .setApplicationVersion(0)
                .setPEngineName(engine->getName().c_str())
                .setEngineVersion(0)
                .setApiVersion(VK_API_VERSION_1_0);
        auto const inst_info = vk::InstanceCreateInfo()
                .setPApplicationInfo(&app)
                .setEnabledLayerCount(enabled_layer_count)
                .setPpEnabledLayerNames(instance_validation_layers)
                .setEnabledExtensionCount(enabled_extension_count)
                .setPpEnabledExtensionNames(extension_names);

        result = vk::createInstance(&inst_info, nullptr, &inst);
        if (result == vk::Result::eErrorIncompatibleDriver) {
            ERR_EXIT(
                    "Cannot find a compatible Vulkan installable client "
                            "driver (ICD).\n\n"
                            "Please look at the Getting Started guide for additional "
                            "information.\n",
                    "vkCreateInstance Failure");
        } else if (result == vk::Result::eErrorExtensionNotPresent) {
            ERR_EXIT(
                    "Cannot find a specified extension library.\n"
                            "Make sure your layers path is set appropriately.\n",
                    "vkCreateInstance Failure");
        } else if (result != vk::Result::eSuccess) {
            ERR_EXIT(
                    "vkCreateInstance failed.\n\n"
                            "Do you have a compatible Vulkan installable client "
                            "driver (ICD) installed?\n"
                            "Please look at the Getting Started guide for additional "
                            "information.\n",
                    "vkCreateInstance Failure");
        }

        /* Make initial call to query gpu_count, then second call for gpu info*/
        uint32_t gpu_count;
        result = inst.enumeratePhysicalDevices(&gpu_count, nullptr);
        VERIFY(result == vk::Result::eSuccess);
        assert(gpu_count > 0);

        if (gpu_count > 0) {
            std::unique_ptr<vk::PhysicalDevice[]> physical_devices(new vk::PhysicalDevice[gpu_count]);
            result = inst.enumeratePhysicalDevices(&gpu_count, physical_devices.get());
            VERIFY(result == vk::Result::eSuccess);
            /* For cube demo we just grab the first physical device */
            gpu = physical_devices[0];
        } else {
            ERR_EXIT(
                    "vkEnumeratePhysicalDevices reported zero accessible "
                            "devices.\n\n"
                            "Do you have a compatible Vulkan installable client "
                            "driver (ICD) installed?\n"
                            "Please look at the Getting Started guide for additional "
                            "information.\n",
                    "vkEnumeratePhysicalDevices Failure");
        }

        /* Look for device extensions */
        uint32_t device_extension_count = 0;
        vk::Bool32 swapchainExtFound = VK_FALSE;
        enabled_extension_count = 0;
        memset(extension_names, 0, sizeof(extension_names));

        result = gpu.enumerateDeviceExtensionProperties(nullptr, &device_extension_count, nullptr);
        VERIFY(result == vk::Result::eSuccess);

        if (device_extension_count > 0) {
            std::unique_ptr<vk::ExtensionProperties[]> device_extensions(new vk::ExtensionProperties[device_extension_count]);
            result = gpu.enumerateDeviceExtensionProperties(nullptr, &device_extension_count, device_extensions.get());
            VERIFY(result == vk::Result::eSuccess);

            for (uint32_t i = 0; i < device_extension_count; i++) {
                if (!strcmp(VK_KHR_SWAPCHAIN_EXTENSION_NAME, device_extensions[i].extensionName)) {
                    swapchainExtFound = 1;
                    extension_names[enabled_extension_count++] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
                }
                assert(enabled_extension_count < 64);
            }
        }

        if (!swapchainExtFound) {
            ERR_EXIT(
                    "vkEnumerateDeviceExtensionProperties failed to find "
                            "the " VK_KHR_SWAPCHAIN_EXTENSION_NAME
            " extension.\n\n"
                    "Do you have a compatible Vulkan installable client "
                    "driver (ICD) installed?\n"
                    "Please look at the Getting Started guide for additional "
                    "information.\n",
                    "vkCreateInstance Failure");
        }

        gpu.getProperties(&gpu_props);

        /* Call with nullptr data to get count */
        gpu.getQueueFamilyProperties(&queue_family_count, nullptr);
        assert(queue_family_count >= 1);

        queue_props.reset(new vk::QueueFamilyProperties[queue_family_count]);
        gpu.getQueueFamilyProperties(&queue_family_count, queue_props.get());

        // Query fine-grained feature support for this device.
        //  If app has specific feature requirements it should check supported
        //  features based on this query
        vk::PhysicalDeviceFeatures physDevFeatures;
        gpu.getFeatures(&physDevFeatures);

}

vk::Bool32 vlk::VulkanModule::checkLayers(uint32_t check_count, char const *const *const check_names, uint32_t layer_count,
                        vk::LayerProperties *layers) {
    for (uint32_t i = 0; i < check_count; i++) {
        vk::Bool32 found = VK_FALSE;
        for (uint32_t j = 0; j < layer_count; j++) {
            if (!strcmp(check_names[i], layers[j].layerName)) {
                found = VK_TRUE;
                break;
            }
        }
        if (!found) {
            fprintf(stderr, "Cannot find layer: %s\n", check_names[i]);
            return 0;
        }
    }
    return VK_TRUE;
}