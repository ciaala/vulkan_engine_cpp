//
// Created by crypt on 16/07/17.
//
#include "renderer/VulkanModule.hpp"
#include "../utility/TimeUtility.hpp"
#include <iostream>

vlk::VulkanModule::VulkanModule(Engine *engine, bool validate) :
    enabled_layer_count{0},
    enabled_extension_count{0},
    use_staging_buffer{true} {
  this->engine = engine;
  this->validate = validate;
  this->pipelineModule = new VulkanPipelineModule(&this->device);
}

void vlk::VulkanModule::initValidation() {
  FLOG(INFO);
  char const *const *instance_validation_layers = nullptr;
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

      validation_found = this->checkLayers(ARRAY_SIZE(instance_validation_layers_alt1),
                                           instance_validation_layers,
                                           instance_layer_count, instance_layers.get());
      if (validation_found) {
        enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt1);
        enabled_layers[0] = "VK_LAYER_LUNARG_standard_validation";
        validation_layer_count = 1;
      } else {
        // use alternative set of validation layers

        FLOG(INFO) << "Trying to use the alternative validation layers" << std::endl;
        instance_validation_layers = instance_validation_layers_alt2;
        enabled_layer_count = ARRAY_SIZE(instance_validation_layers_alt2);
        validation_found = this->checkLayers(ARRAY_SIZE(instance_validation_layers_alt2),
                                             instance_validation_layers,
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

void vlk::VulkanModule::initDevice() {
  FLOG(INFO);

  uint32_t instance_extension_count = 0;

  enabled_extension_count = 0;

  // Look for validation layers
  this->initValidation();

  /* Look for instance extensions */
  vk::Bool32 surfaceExtFound = VK_FALSE;
  vk::Bool32 platformSurfaceExtFound = VK_FALSE;
  memset(extension_names, 0, sizeof(extension_names));

  auto result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count, nullptr);
  VERIFY(result == vk::Result::eSuccess);

  if (instance_extension_count > 0) {
    std::unique_ptr<vk::ExtensionProperties[]> instance_extensions(
        new vk::ExtensionProperties[instance_extension_count]);
    result = vk::enumerateInstanceExtensionProperties(nullptr, &instance_extension_count,
                                                      instance_extensions.get());
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
            "the "
        VK_KHR_SURFACE_EXTENSION_NAME
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
            "the "
        VK_KHR_XCB_SURFACE_EXTENSION_NAME
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
  auto name = engine->getApplication()->getName();
  auto const app = vk::ApplicationInfo()
      .setPApplicationName("stoCazzo")
      .setApplicationVersion(0)
      .setPEngineName(engine->getName().c_str())
      .setEngineVersion(0)
      .setApiVersion(VK_API_VERSION_1_0);
  auto const inst_info = vk::InstanceCreateInfo()
      .setPApplicationInfo(&app)
      .setEnabledLayerCount(enabled_layer_count)
      .setPpEnabledLayerNames(enabled_layers)
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
    std::unique_ptr<vk::ExtensionProperties[]> device_extensions(
        new vk::ExtensionProperties[device_extension_count]);
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
            "the "
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

vk::Bool32
vlk::VulkanModule::checkLayers(uint32_t check_count, char const *const *const check_names, uint32_t layer_count,
                               vk::LayerProperties *layers) {
  FLOG(INFO);
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

void vlk::VulkanModule::initSurface(xcb_connection_t *connection, xcb_window_t xcb_window) {
  FLOG(INFO);
// Create a WSI surface for the window:
#if defined(VK_USE_PLATFORM_WIN32_KHR)
  {
          auto const createInfo = vk::Win32SurfaceCreateInfoKHR().setHinstance(connection).setHwnd(window);

          auto result = inst.createWin32SurfaceKHR(&createInfo, nullptr, &surface);
          VERIFY(result == vk::Result::eSuccess);
      }
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
  {
          auto const createInfo = vk::WaylandSurfaceCreateInfoKHR().setDisplay(display).setSurface(window);

          auto result = inst.createWaylandSurfaceKHR(&createInfo, nullptr, &surface);
          VERIFY(result == vk::Result::eSuccess);
      }
#elif defined(VK_USE_PLATFORM_MIR_KHR)
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
  {
          auto const createInfo = vk::XlibSurfaceCreateInfoKHR().setDpy(display).setWindow(xlib_window);

          auto result = inst.createXlibSurfaceKHR(&createInfo, nullptr, &surface);
          VERIFY(result == vk::Result::eSuccess);
      }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
  {
    auto const createInfo = vk::XcbSurfaceCreateInfoKHR().setConnection(connection).setWindow(xcb_window);

    auto result = inst.createXcbSurfaceKHR(&createInfo, nullptr, &this->surface);
    VERIFY(result == vk::Result::eSuccess);
  }
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
  {
          auto result = create_display_surface();
          VERIFY(result == vk::Result::eSuccess);
      }
#endif
}

void vlk::VulkanModule::initSwapChain() {
  FLOG(INFO);
  // Iterate over each queue to learn whether it supports presenting:
  std::unique_ptr<vk::Bool32[]> supportsPresent(new vk::Bool32[queue_family_count]);
  for (uint32_t i = 0; i < queue_family_count; i++) {
    gpu.getSurfaceSupportKHR(i, surface, &supportsPresent[i]);
  }

  uint32_t graphicsQueueFamilyIndex = UINT32_MAX;
  uint32_t presentQueueFamilyIndex = UINT32_MAX;
  for (uint32_t i = 0; i < queue_family_count; i++) {
    if (queue_props[i].queueFlags & vk::QueueFlagBits::eGraphics) {
      if (graphicsQueueFamilyIndex == UINT32_MAX) {
        graphicsQueueFamilyIndex = i;
      }

      if (supportsPresent[i] == VK_TRUE) {
        graphicsQueueFamilyIndex = i;
        presentQueueFamilyIndex = i;
        break;
      }
    }
  }

  if (presentQueueFamilyIndex == UINT32_MAX) {
    // If didn't find a queue that supports both graphics and present,
    // then
    // find a separate present queue.
    for (uint32_t i = 0; i < queue_family_count; ++i) {
      if (supportsPresent[i] == VK_TRUE) {
        presentQueueFamilyIndex = i;
        break;
      }
    }
  }

  // Generate error if could not find both a graphics and a present queue
  if (graphicsQueueFamilyIndex == UINT32_MAX || presentQueueFamilyIndex == UINT32_MAX) {
    ERR_EXIT("Could not find both graphics and present queues\n", "Swapchain Initialization Failure");
  }

  graphics_queue_family_index = graphicsQueueFamilyIndex;
  present_queue_family_index = presentQueueFamilyIndex;
  separate_present_queue = (graphics_queue_family_index != present_queue_family_index);

  this->createDevice();

  device.getQueue(graphics_queue_family_index, 0, &graphics_queue);
  if (!separate_present_queue) {
    present_queue = graphics_queue;
  } else {
    FLOG(INFO) << "Graphic and Pesentation queue are separated.";
    device.getQueue(present_queue_family_index, 0, &present_queue);
  }

  // Get the list of VkFormat's that are supported:
  uint32_t formatCount;
  auto result = gpu.getSurfaceFormatsKHR(surface, &formatCount, nullptr);
  VERIFY(result == vk::Result::eSuccess);

  std::unique_ptr<vk::SurfaceFormatKHR[]> surfFormats(new vk::SurfaceFormatKHR[formatCount]);
  result = gpu.getSurfaceFormatsKHR(surface, &formatCount, surfFormats.get());
  VERIFY(result == vk::Result::eSuccess);

  // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
  // the surface has no preferred format.  Otherwise, at least one
  // supported format will be returned.
  if (formatCount == 1 && surfFormats[0].format == vk::Format::eUndefined) {
    format = vk::Format::eB8G8R8A8Unorm;
  } else {
    assert(formatCount >= 1);
    format = surfFormats[0].format;
  }
  color_space = surfFormats[0].colorSpace;

  quit = false;
  curFrame = 0;

  // Create semaphores to synchronize acquiring presentable buffers before
  // rendering and waiting for drawing to be complete before presenting
  auto const semaphoreCreateInfo = vk::SemaphoreCreateInfo();

  // Create fences that we can use to throttle if we get too far
  // ahead of the image presents
  auto const fence_ci = vk::FenceCreateInfo().setFlags(vk::FenceCreateFlagBits::eSignaled);
  for (uint32_t i = 0; i < FRAME_LAG; i++) {
    result = device.createFence(&fence_ci, nullptr, &fences[i]);
    VERIFY(result == vk::Result::eSuccess);

    result = device.createSemaphore(&semaphoreCreateInfo, nullptr, &image_acquired_semaphores[i]);
    VERIFY(result == vk::Result::eSuccess);

    result = device.createSemaphore(&semaphoreCreateInfo, nullptr, &draw_complete_semaphores[i]);
    VERIFY(result == vk::Result::eSuccess);

    if (separate_present_queue) {
      result = device.createSemaphore(&semaphoreCreateInfo, nullptr, &image_ownership_semaphores[i]);
      VERIFY(result == vk::Result::eSuccess);
    }
  }
  frame_index = 0;

  // Get Memory information and properties
  gpu.getMemoryProperties(&memory_properties);
}

void vlk::VulkanModule::createDevice() {
  FLOG(INFO);
  float const priorities[1] = {0.0};

  vk::DeviceQueueCreateInfo queues[2];
  queues[0].setQueueFamilyIndex(graphics_queue_family_index);
  queues[0].setQueueCount(1);
  queues[0].setPQueuePriorities(priorities);

  auto deviceInfo = vk::DeviceCreateInfo()
      .setQueueCreateInfoCount(1)
      .setPQueueCreateInfos(queues)
      .setEnabledLayerCount(0)
      .setPpEnabledLayerNames(nullptr)
      .setEnabledExtensionCount(enabled_extension_count)
      .setPpEnabledExtensionNames((const char *const *) extension_names)
      .setPEnabledFeatures(nullptr);

  if (separate_present_queue) {
    queues[1].setQueueFamilyIndex(present_queue_family_index);
    queues[1].setQueueCount(1);
    queues[1].setPQueuePriorities(priorities);
    deviceInfo.setQueueCreateInfoCount(2);
  }

  auto result = gpu.createDevice(&deviceInfo, nullptr, &device);
  VERIFY(result == vk::Result::eSuccess);
  this->memoryModule = new MemoryModule(&memory_properties);
  this->textureModule = new TextureModule(&device, memoryModule);
  this->shaderModule = new ShaderModule(&device);
  this->pipelineModule = new VulkanPipelineModule(&device);
}

void vlk::VulkanModule::prepare() {
  FLOG(INFO);
  this->width = 500;
  this->height = 500;

  auto const cmd_pool_info = vk::CommandPoolCreateInfo()
      .setQueueFamilyIndex(graphics_queue_family_index);
  auto result = device
      .createCommandPool(&cmd_pool_info, nullptr, &cmd_pool);
  VERIFY(result == vk::Result::eSuccess);
  this->preparePrimaryCommandBuffer(&this->cmd);

  VERIFY(result == vk::Result::eSuccess);

  auto const cmd_buf_info = vk::CommandBufferBeginInfo().setPInheritanceInfo(nullptr);

  result = this->cmd.begin(&cmd_buf_info);
  VERIFY(result == vk::Result::eSuccess);

  this->prepareSwapchainBuffers();

  this->prepareDepth();

  // Initialize texture properties;
  this->prepareTextureFormats();
  this->prepareRenderPassAndFramebuffer();

}

void vlk::VulkanModule::prepareSwapchainBuffers() {
  FLOG(INFO);
  vk::SwapchainKHR oldSwapchain = swapchain;

  // Check the surface capabilities and formats
  vk::SurfaceCapabilitiesKHR surfCapabilities;
  auto result = gpu.getSurfaceCapabilitiesKHR(surface, &surfCapabilities);
  VERIFY(result == vk::Result::eSuccess);

  uint32_t presentModeCount;
  result = gpu.getSurfacePresentModesKHR(surface, &presentModeCount, nullptr);
  VERIFY(result == vk::Result::eSuccess);

  std::unique_ptr<vk::PresentModeKHR[]> presentModes(new vk::PresentModeKHR[presentModeCount]);
  result = gpu.getSurfacePresentModesKHR(surface, &presentModeCount, presentModes.get());
  VERIFY(result == vk::Result::eSuccess);

  vk::Extent2D swapchainExtent;
  // width and height are either both -1, or both not -1.
  if (surfCapabilities.currentExtent.width == (uint32_t) -1) {
    // If the surface size is undefined, the size is set to
    // the size of the images requested.
    swapchainExtent.width = width;
    swapchainExtent.height = height;
  } else {
    // If the surface size is defined, the swap chain size must match
    swapchainExtent = surfCapabilities.currentExtent;
    width = surfCapabilities.currentExtent.width;
    height = surfCapabilities.currentExtent.height;
  }

  // The FIFO present mode is guaranteed by the spec to be supported
  // and to have no tearing.  It's a great default present mode to use.
  vk::PresentModeKHR swapchainPresentMode = vk::PresentModeKHR::eFifo;

  //  There are times when you may wish to use another present mode.  The
  //  following code shows how to select them, and the comments provide some
  //  reasons you may wish to use them.
  //
  // It should be noted that Vulkan 1.0 doesn't provide a method for
  // synchronizing rendering with the presentation engine's display.  There
  // is a method provided for throttling rendering with the display, but
  // there are some presentation engines for which this method will not work.
  // If an application doesn't throttle its rendering, and if it renders much
  // faster than the refresh rate of the display, this can waste power on
  // mobile devices.  That is because power is being spent rendering images
  // that may never be seen.

  // VK_PRESENT_MODE_IMMEDIATE_KHR is for applications that don't care
  // about
  // tearing, or have some way of synchronizing their rendering with the
  // display.
  // VK_PRESENT_MODE_MAILBOX_KHR may be useful for applications that
  // generally render a new presentable image every refresh cycle, but are
  // occasionally early.  In this case, the application wants the new
  // image
  // to be displayed instead of the previously-queued-for-presentation
  // image
  // that has not yet been displayed.
  // VK_PRESENT_MODE_FIFO_RELAXED_KHR is for applications that generally
  // render a new presentable image every refresh cycle, but are
  // occasionally
  // late.  In this case (perhaps because of stuttering/latency concerns),
  // the application wants the late image to be immediately displayed,
  // even
  // though that may mean some tearing.

  if (presentMode != swapchainPresentMode) {
    for (size_t i = 0; i < presentModeCount; ++i) {
      if (presentModes[i] == presentMode) {
        swapchainPresentMode = presentMode;
        break;
      }
    }
  }

  if (swapchainPresentMode != presentMode) {
    ERR_EXIT("Present mode specified is not supported\n", "Present mode unsupported");
  }

  // Determine the number of VkImages to use in the swap chain.
  // Application desires to acquire 3 images at a time for triple
  // buffering
  uint32_t desiredNumOfSwapchainImages = 3;
  if (desiredNumOfSwapchainImages < surfCapabilities.minImageCount) {
    desiredNumOfSwapchainImages = surfCapabilities.minImageCount;
  }

  // If maxImageCount is 0, we can ask for as many images as we want,
  // otherwise
  // we're limited to maxImageCount
  if ((surfCapabilities.maxImageCount > 0) && (desiredNumOfSwapchainImages > surfCapabilities.maxImageCount)) {
    // Application must settle for fewer images than desired:
    desiredNumOfSwapchainImages = surfCapabilities.maxImageCount;
  }

  vk::SurfaceTransformFlagBitsKHR preTransform;
  if (surfCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
    preTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
  } else {
    preTransform = surfCapabilities.currentTransform;
  }

  // Find a supported composite alpha mode - one of these is guaranteed to be set
  vk::CompositeAlphaFlagBitsKHR compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
  vk::CompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
      vk::CompositeAlphaFlagBitsKHR::eOpaque,
      vk::CompositeAlphaFlagBitsKHR::ePreMultiplied,
      vk::CompositeAlphaFlagBitsKHR::ePostMultiplied,
      vk::CompositeAlphaFlagBitsKHR::eInherit,
  };
  for (uint32_t i = 0; i < sizeof(compositeAlphaFlags); i++) {
    if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
      compositeAlpha = compositeAlphaFlags[i];
      break;
    }
  }

  auto const swapchain_ci = vk::SwapchainCreateInfoKHR()
      .setSurface(surface)
      .setMinImageCount(desiredNumOfSwapchainImages)
      .setImageFormat(format)
      .setImageColorSpace(color_space)
      .setImageExtent({swapchainExtent.width, swapchainExtent.height})
      .setImageArrayLayers(1)
      .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
      .setImageSharingMode(vk::SharingMode::eExclusive)
      .setQueueFamilyIndexCount(0)
      .setPQueueFamilyIndices(nullptr)
      .setPreTransform(preTransform)
      .setCompositeAlpha(compositeAlpha)
      .setPresentMode(swapchainPresentMode)
      .setClipped(true)
      .setOldSwapchain(oldSwapchain);

  result = device.createSwapchainKHR(&swapchain_ci, nullptr, &swapchain);
  VERIFY(result == vk::Result::eSuccess);

  // If we just re-created an existing swapchain, we should destroy the
  // old
  // swapchain at this point.
  // Note: destroying the swapchain also cleans up all its associated
  // presentable images once the platform is done with them.
  if (oldSwapchain) {
    device.destroySwapchainKHR(oldSwapchain, nullptr);
  }

  result = device.getSwapchainImagesKHR(swapchain, &swapchainImageCount, nullptr);
  VERIFY(result == vk::Result::eSuccess);

  std::unique_ptr<vk::Image[]> swapchainImages(new vk::Image[swapchainImageCount]);
  result = device.getSwapchainImagesKHR(swapchain, &swapchainImageCount, swapchainImages.get());
  VERIFY(result == vk::Result::eSuccess);

  swapchain_image_resources.reset(new SwapchainImageResources[swapchainImageCount]);

  for (uint32_t index = 0; index < swapchainImageCount; ++index) {
    auto result = this->prepareImageToView(swapchainImages[index], index);
    VERIFY(result == vk::Result::eSuccess);
    this->prepareSwapchainCommandBuffer(index);

  }
}

vk::Result vlk::VulkanModule::prepareImageToView(const vk::Image &image, uint32_t index) {
  FLOG(INFO);
  auto color_image_view =
      vk::ImageViewCreateInfo()
          .setViewType(vk::ImageViewType::e2D)
          .setFormat(this->format)
          .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  this->swapchain_image_resources[index].image = image;

  color_image_view.image = image;

  return this->device.createImageView(&color_image_view, nullptr, &this->swapchain_image_resources[index].view);

}

vk::Result vlk::VulkanModule::prepareSwapchainCommandBuffer(uint32_t index) {
  FLOG(INFO);
  auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(this->cmd_pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);

  return device.allocateCommandBuffers(&commandBufferAllocateInfo,
                                       &this->swapchain_image_resources[index].cmd);
}

void vlk::VulkanModule::prepareDepth() {
  FLOG(INFO);

  depth.format = vk::Format::eD16Unorm;

  auto const image = vk::ImageCreateInfo()
      .setImageType(vk::ImageType::e2D)
      .setFormat(depth.format)
      .setExtent({(uint32_t) width, (uint32_t) height, 1})
      .setMipLevels(1)
      .setArrayLayers(1)
      .setSamples(vk::SampleCountFlagBits::e1)
      .setTiling(vk::ImageTiling::eOptimal)
      .setUsage(vk::ImageUsageFlagBits::eDepthStencilAttachment)
      .setSharingMode(vk::SharingMode::eExclusive)
      .setQueueFamilyIndexCount(0)
      .setPQueueFamilyIndices(nullptr)
      .setInitialLayout(vk::ImageLayout::eUndefined);

  auto result = device.createImage(&image, nullptr, &depth.image);
  VERIFY(result == vk::Result::eSuccess);

  vk::MemoryRequirements mem_reqs;
  device.getImageMemoryRequirements(depth.image, &mem_reqs);

  depth.mem_alloc.setAllocationSize(mem_reqs.size);
  depth.mem_alloc.setMemoryTypeIndex(0);

  auto const pass = memoryModule->memoryTypeFromProperties(mem_reqs.memoryTypeBits,
                                                           vk::MemoryPropertyFlagBits::eDeviceLocal,
                                                           &depth.mem_alloc.memoryTypeIndex);
  VERIFY(pass);

  result = device.allocateMemory(&depth.mem_alloc, nullptr, &depth.mem);
  VERIFY(result == vk::Result::eSuccess);
  //TODO Investigate this change to the result type. Is it an API change ?
  // result =
  device.bindImageMemory(depth.image, depth.mem, 0);
  //VERIFY(result == vk::Result::eSuccess);

  auto const view = vk::ImageViewCreateInfo()
      .setImage(depth.image)
      .setViewType(vk::ImageViewType::e2D)
      .setFormat(depth.format)
      .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eDepth, 0, 1, 0, 1));
  result = device.createImageView(&view, nullptr, &depth.view);
  VERIFY(result == vk::Result::eSuccess);
}

void vlk::VulkanModule::prepareTextureFormats() {
  gpu.getFormatProperties(textureFormat, &textureFormatProperties);
}

//TODO MOVE to TextureModule
void vlk::VulkanModule::prepareTexture(const char *textureFile, const vk::Format &tex_format) {
  FLOG(INFO);

  texture_object currentTexture;
  if ((this->textureFormatProperties.linearTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) &&
      !this->use_staging_buffer) {
    /* Device can texture using linear textures */
    this->textureModule->prepareTextureImage(textureFile, currentTexture, vk::ImageTiling::eLinear,
                                             vk::ImageUsageFlagBits::eSampled,
                                             vk::MemoryPropertyFlagBits::eHostVisible |
                                                 vk::MemoryPropertyFlagBits::eHostCoherent);
    // Nothing in the globalPipeline needs to be complete to start, and don't allow fragment
    // shader to run until layout transition completes
    this->textureModule->setImageLayout(&this->cmd,
                                        currentTexture.image,
                                        vk::ImageAspectFlagBits::eColor,
                                        vk::ImageLayout::ePreinitialized,
                                        currentTexture.imageLayout,
                                        vk::AccessFlagBits::eHostWrite,
                                        vk::PipelineStageFlagBits::eTopOfPipe,
                                        vk::PipelineStageFlagBits::eFragmentShader);
    this->staging_texture.image = vk::Image();
  } else if (this->textureFormatProperties.optimalTilingFeatures & vk::FormatFeatureFlagBits::eSampledImage) {
    /* Must use staging buffer to copy linear texture to optimized */

    this->textureModule->prepareTextureImage(textureFile,
                                             this->staging_texture,
                                             vk::ImageTiling::eLinear,
                                             vk::ImageUsageFlagBits::eTransferSrc,
                                             vk::MemoryPropertyFlagBits::eHostVisible
                                                 | vk::MemoryPropertyFlagBits::eHostCoherent);

    this->textureModule->prepareTextureImage(textureFile,
                                             currentTexture,
                                             vk::ImageTiling::eOptimal,
                                             vk::ImageUsageFlagBits::eTransferDst
                                                 | vk::ImageUsageFlagBits::eSampled,
                                             vk::MemoryPropertyFlagBits::eDeviceLocal);

    this->textureModule->setImageLayout(&this->cmd,
                                        this->staging_texture.image,
                                        vk::ImageAspectFlagBits::eColor,
                                        vk::ImageLayout::ePreinitialized,
                                        vk::ImageLayout::eTransferSrcOptimal,
                                        vk::AccessFlagBits::eHostWrite,
                                        vk::PipelineStageFlagBits::eTopOfPipe,
                                        vk::PipelineStageFlagBits::eTransfer);

    this->textureModule->setImageLayout(&this->cmd,
                                        currentTexture.image,
                                        vk::ImageAspectFlagBits::eColor,
                                        vk::ImageLayout::ePreinitialized,
                                        vk::ImageLayout::eTransferDstOptimal,
                                        vk::AccessFlagBits::eHostWrite,
                                        vk::PipelineStageFlagBits::eTopOfPipe,
                                        vk::PipelineStageFlagBits::eTransfer);

    auto const subresource = vk::ImageSubresourceLayers()
        .setAspectMask(vk::ImageAspectFlagBits::eColor)
        .setMipLevel(0)
        .setBaseArrayLayer(0)
        .setLayerCount(1);

    auto const copy_region =
        vk::ImageCopy()
            .setSrcSubresource(subresource)
            .setSrcOffset({0, 0, 0})
            .setDstSubresource(subresource)
            .setDstOffset({0, 0, 0})
            .setExtent(
                {(uint32_t) this->staging_texture.tex_width,
                 (uint32_t) this->staging_texture.tex_height, 1});

    this->cmd.copyImage(this->staging_texture.image, vk::ImageLayout::eTransferSrcOptimal, currentTexture.image,
                        vk::ImageLayout::eTransferDstOptimal, 1, &copy_region);

    this->textureModule->setImageLayout(&this->cmd, currentTexture.image, vk::ImageAspectFlagBits::eColor,
                                        vk::ImageLayout::eTransferDstOptimal,
                                        currentTexture.imageLayout, vk::AccessFlagBits::eTransferWrite,
                                        vk::PipelineStageFlagBits::eTransfer,
                                        vk::PipelineStageFlagBits::eFragmentShader);
  } else {
    std::cerr << "No support for R8G8B8A8_UNORM as texture image format";
  }

  auto const samplerInfo = vk::SamplerCreateInfo()
      .setMagFilter(vk::Filter::eNearest)
      .setMinFilter(vk::Filter::eNearest)
      .setMipmapMode(vk::SamplerMipmapMode::eNearest)
      .setAddressModeU(vk::SamplerAddressMode::eClampToEdge)
      .setAddressModeV(vk::SamplerAddressMode::eClampToEdge)
      .setAddressModeW(vk::SamplerAddressMode::eClampToEdge)
      .setMipLodBias(0.0f)
      .setAnisotropyEnable(VK_FALSE)
      .setMaxAnisotropy(1)
      .setCompareEnable(VK_FALSE)
      .setCompareOp(vk::CompareOp::eNever)
      .setMinLod(0.0f)
      .setMaxLod(0.0f)
      .setBorderColor(vk::BorderColor::eFloatOpaqueWhite)
      .setUnnormalizedCoordinates(VK_FALSE);

  auto result = this->device.createSampler(&samplerInfo, nullptr, &currentTexture.sampler);
  VERIFY(result == vk::Result::eSuccess);

  auto const viewInfo = vk::ImageViewCreateInfo()
      .setImage(currentTexture.image)
      .setViewType(vk::ImageViewType::e2D)
      .setFormat(tex_format)
      .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  result = this->device.createImageView(&viewInfo, nullptr, &currentTexture.view);
  VERIFY(result == vk::Result::eSuccess);
  this->textures.emplace_back(currentTexture);
}

void vlk::VulkanModule::prepareCubeDataBuffers(Camera *camera, GameObject *object) {
  FLOG(INFO);

  mat4x4 VP{0};
  mat4x4_mul(VP, camera->getProjectionMatrix(), camera->getViewMatrix());

  mat4x4 MVP{0};
  mat4x4_mul(MVP, VP, object->getModelMatrix());

  vktexcube_vs_uniform data{0};
  memcpy(data.mvp, MVP, sizeof(MVP));
  //    dumpMatrix("MVP", MVP)

  for (int32_t i = 0; i < 12 * 3; i++) {

    data.position[i][0] = object->getVertexBufferData()[i * 3];
    data.position[i][1] = object->getVertexBufferData()[i * 3 + 1];
    data.position[i][2] = object->getVertexBufferData()[i * 3 + 2];
    data.position[i][3] = 1.0f;
    data.attr[i][0] = object->getUVBufferData()[2 * i];
    data.attr[i][1] = object->getUVBufferData()[2 * i + 1];
    data.attr[i][2] = 0;
    data.attr[i][3] = 0;
  }

  auto const buf_info = vk::BufferCreateInfo().setSize(sizeof(data)).setUsage(
      vk::BufferUsageFlagBits::eUniformBuffer);

  for (unsigned int i = 0; i < swapchainImageCount; i++) {
    auto result = device.createBuffer(&buf_info, nullptr, &swapchain_image_resources[i].uniform_buffer);
    VERIFY(result == vk::Result::eSuccess);

    vk::MemoryRequirements mem_reqs;
    device.getBufferMemoryRequirements(swapchain_image_resources[i].uniform_buffer, &mem_reqs);

    auto mem_alloc = vk::MemoryAllocateInfo().setAllocationSize(mem_reqs.size).setMemoryTypeIndex(0);

    bool const pass = memoryModule->memoryTypeFromProperties(
        mem_reqs.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        &mem_alloc.memoryTypeIndex);
    VERIFY(pass);

    result = device.allocateMemory(&mem_alloc, nullptr, &swapchain_image_resources[i].uniform_memory);
    VERIFY(result == vk::Result::eSuccess);

    auto pData = device.mapMemory(swapchain_image_resources[i].uniform_memory, 0, VK_WHOLE_SIZE,
                                  vk::MemoryMapFlags());
    // TODO Investigate
    //VERIFY(pData.result == vk::Result::eSuccess);

    memcpy(pData, &data, sizeof data);

    device.unmapMemory(swapchain_image_resources[i].uniform_memory);
    // TODO Investigate
    // result =
    device.bindBufferMemory(swapchain_image_resources[i].uniform_buffer,
                            swapchain_image_resources[i].uniform_memory, 0);
    // VERIFY(result == vk::Result::eSuccess);
  }
}

void vlk::VulkanModule::prepareDescriptorPool() {
  FLOG(INFO);
  // TODO TextureCount
  uint32_t texture_count = 2;
  vk::DescriptorPoolSize const poolSizes[2] = {
      vk::DescriptorPoolSize().setType(vk::DescriptorType::eUniformBuffer).setDescriptorCount(
          swapchainImageCount),
      vk::DescriptorPoolSize().setType(vk::DescriptorType::eCombinedImageSampler).setDescriptorCount(
          swapchainImageCount * texture_count)};

  auto const descriptor_pool = vk::DescriptorPoolCreateInfo().setMaxSets(swapchainImageCount).setPoolSizeCount(
      2).setPPoolSizes(poolSizes);

  auto result = device.createDescriptorPool(&descriptor_pool, nullptr, &desc_pool);
  VERIFY(result == vk::Result::eSuccess);
}

void vlk::VulkanModule::prepareDescriptorSet() {
  FLOG(INFO);
  auto const alloc_info =
      vk::DescriptorSetAllocateInfo()
          .setDescriptorPool(desc_pool)
          .setDescriptorSetCount(1)
          .setPSetLayouts(&desc_layout);

  auto buffer_info = vk::DescriptorBufferInfo()
      .setOffset(0)
      .setRange(sizeof(struct vktexcube_vs_uniform));

  vk::DescriptorImageInfo tex_descs[textures.size()];
  for (uint32_t i = 0; i < textures.size(); i++) {
    tex_descs[i].setSampler(textures[i].sampler);
    tex_descs[i].setImageView(textures[i].view);
    tex_descs[i].setImageLayout(vk::ImageLayout::eGeneral);
  }

  vk::WriteDescriptorSet writes[2];

  writes[0].setDescriptorCount(1);
  writes[0].setDescriptorType(vk::DescriptorType::eUniformBuffer);
  writes[0].setPBufferInfo(&buffer_info);

  writes[1].setDstBinding(1);
  writes[1].setDescriptorCount((uint32_t) textures.size());
  writes[1].setDescriptorType(vk::DescriptorType::eCombinedImageSampler);
  writes[1].setPImageInfo(tex_descs);

  for (unsigned int i = 0; i < swapchainImageCount; i++) {
    auto result = device.allocateDescriptorSets(&alloc_info, &swapchain_image_resources[i].descriptor_set);
    VERIFY(result == vk::Result::eSuccess);

    buffer_info.setBuffer(swapchain_image_resources[i].uniform_buffer);
    writes[0].setDstSet(swapchain_image_resources[i].descriptor_set);
    writes[1].setDstSet(swapchain_image_resources[i].descriptor_set);
    device.updateDescriptorSets(2, writes, 0, nullptr);
  }
}

void vlk::VulkanModule::prepareFramebuffers() {
  FLOG(INFO);
  vk::ImageView attachments[2];
  attachments[1] = depth.view;

  auto const fb_info = vk::FramebufferCreateInfo()
      .setRenderPass(render_pass)
      .setAttachmentCount(2)
      .setPAttachments(attachments)
      .setWidth((uint32_t) width)
      .setHeight((uint32_t) height)
      .setLayers(1);

  for (uint32_t i = 0; i < swapchainImageCount; i++) {
    attachments[0] = swapchain_image_resources[i].view;
    auto const result = device.createFramebuffer(&fb_info, nullptr, &swapchain_image_resources[i].framebuffer);
    VERIFY(result == vk::Result::eSuccess);
  }
}

void vlk::VulkanModule::prepareRenderPass() {
  FLOG(INFO);
  // The initial layout for the color and depth attachments will be LAYOUT_UNDEFINED
  // because at the start of the renderpass, we don't care about their contents.
  // At the start of the subpass, the color attachment's layout will be transitioned
  // to LAYOUT_COLOR_ATTACHMENT_OPTIMAL and the depth stencil attachment's layout
  // will be transitioned to LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL.  At the end of
  // the renderpass, the color attachment's layout will be transitioned to
  // LAYOUT_PRESENT_SRC_KHR to be ready to present.  This is all done as part of
  // the renderpass, no barriers are necessary.
  const vk::AttachmentDescription attachments[2] = {vk::AttachmentDescription()
                                                        .setFormat(format)
                                                        .setSamples(vk::SampleCountFlagBits::e1)
                                                        .setLoadOp(vk::AttachmentLoadOp::eClear)
                                                        .setStoreOp(vk::AttachmentStoreOp::eStore)
                                                        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                                                        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                                                        .setInitialLayout(vk::ImageLayout::eUndefined)
                                                        .setFinalLayout(vk::ImageLayout::ePresentSrcKHR),
                                                    vk::AttachmentDescription()
                                                        .setFormat(depth.format)
                                                        .setSamples(vk::SampleCountFlagBits::e1)
                                                        .setLoadOp(vk::AttachmentLoadOp::eClear)
                                                        .setStoreOp(vk::AttachmentStoreOp::eDontCare)
                                                        .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
                                                        .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare)
                                                        .setInitialLayout(vk::ImageLayout::eUndefined)
                                                        .setFinalLayout(
                                                            vk::ImageLayout::eDepthStencilAttachmentOptimal)};

  auto const color_reference = vk::AttachmentReference().setAttachment(0).setLayout(
      vk::ImageLayout::eColorAttachmentOptimal);

  auto const depth_reference =
      vk::AttachmentReference().setAttachment(1).setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

  auto const subpass = vk::SubpassDescription()
      .setPipelineBindPoint(vk::PipelineBindPoint::eGraphics)
      .setInputAttachmentCount(0)
      .setPInputAttachments(nullptr)
      .setColorAttachmentCount(1)
      .setPColorAttachments(&color_reference)
      .setPResolveAttachments(nullptr)
      .setPDepthStencilAttachment(&depth_reference)
      .setPreserveAttachmentCount(0)
      .setPPreserveAttachments(nullptr);

  auto const rp_info = vk::RenderPassCreateInfo()
      .setAttachmentCount(2)
      .setPAttachments(attachments)
      .setSubpassCount(1)
      .setPSubpasses(&subpass)
      .setDependencyCount(0)
      .setPDependencies(nullptr);

  auto result = device.createRenderPass(&rp_info, nullptr, &render_pass);
  VERIFY(result == vk::Result::eSuccess);
}

void vlk::VulkanModule::clearBackgroundCommandBuffer(
    vk::CommandBuffer &commandBuffer,
    vk::Framebuffer &frameBuffer) {
  FLOG(INFO);
  auto const inheritanceInfo = vk::CommandBufferInheritanceInfo(render_pass,
                                                                0,
                                                                frameBuffer);
  auto const commandInfo = vk::CommandBufferBeginInfo()
      .setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse)
      .setPInheritanceInfo(&inheritanceInfo);

  float blueIndex = ((curFrame % 25 )*11) / 255.0f;
  float greenIndex = ((curFrame % (25*25))* 121) / (255.0f*255.0f);
  float redIndex = ((curFrame % (25*25*25)) * 11*121) / (255.0f*255.0f*255.0f);
  vk::ClearValue const clearValues[2] = {vk::ClearColorValue(std::array<float, 4>({{redIndex, greenIndex, blueIndex, 1.0f}})),
                                         vk::ClearDepthStencilValue(1.0f, 0u)};

  auto const passInfo = vk::RenderPassBeginInfo()
      .setRenderPass(render_pass)
      .setFramebuffer(frameBuffer)
      .setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D((uint32_t) width, (uint32_t) height)))
      .setClearValueCount(2)
      .setPClearValues(clearValues);

  auto result = commandBuffer.begin(&commandInfo);
  VERIFY(result == vk::Result::eSuccess);

  commandBuffer.beginRenderPass(&passInfo, vk::SubpassContents::eInline);

  auto const viewport =
      vk::Viewport()
          .setWidth((float) width)
          .setHeight((float) height)
          .setMinDepth(0.0f)
          .setMaxDepth(1.0f);
  commandBuffer.setViewport(0, 1, &viewport);

  vk::Rect2D const scissor(vk::Offset2D(0, 0), vk::Extent2D(width, height));
  commandBuffer.setScissor(0, 1, &scissor);
  // commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, this->globalPipeline);

  commandBuffer.endRenderPass();

  if (separate_present_queue) {
    LOG(INFO) << "Separated presentation queue" << std::endl;
    // We have to transfer ownership from the graphics queue family to
    // the
    // present queue family to be able to present.  Note that we don't
    // have
    // to transfer from present queue family back to graphics queue
    // family at
    // the start of the next frame because we don't care about the
    // image's
    // contents at that point.
    auto const image_ownership_barrier =
        vk::ImageMemoryBarrier()
            .setSrcAccessMask(vk::AccessFlags())
            .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
            .setOldLayout(vk::ImageLayout::ePresentSrcKHR)
            .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
            .setSrcQueueFamilyIndex(graphics_queue_family_index)
            .setDstQueueFamilyIndex(present_queue_family_index)
            .setImage(swapchain_image_resources[current_buffer].image)
            .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

    commandBuffer.pipelineBarrier(vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                  vk::PipelineStageFlagBits::eBottomOfPipe, vk::DependencyFlagBits(), 0, nullptr, 0,
                                  nullptr, 1, &image_ownership_barrier);
  }
  // TODO Investigate
  //    result =
  commandBuffer.end();
  //    VERIFY(result == vk::Result::eSuccess);
}

void vlk::VulkanModule::flushInitCmd() {
  // TODO: hmm.
  // This function could get called twice if the texture uses a staging
  // buffer
  // In that case the second call should be ignored
  if (!cmd) {
    return;
  }

  // TODO Investigate this change
  //auto result =
  cmd.end();
  //VERIFY(result == vk::Result::eSuccess);

  auto const fenceInfo = vk::FenceCreateInfo();
  vk::Fence fence;
  auto result = device.createFence(&fenceInfo, nullptr, &fence);
  VERIFY(result == vk::Result::eSuccess);

  vk::CommandBuffer const commandBuffers[] = {cmd};
  auto const submitInfo = vk::SubmitInfo().setCommandBufferCount(1).setPCommandBuffers(commandBuffers);

  result = graphics_queue.submit(1, &submitInfo, fence);
  VERIFY(result == vk::Result::eSuccess);

  result = device.waitForFences(1, &fence, VK_TRUE, UINT64_MAX);
  VERIFY(result == vk::Result::eSuccess);

  device.freeCommandBuffers(cmd_pool, 1, commandBuffers);
  device.destroyFence(fence, nullptr);

  cmd = vk::CommandBuffer();
}

void vlk::VulkanModule::buildImageOwnershipCmd(uint32_t const &index) {
  FLOG(INFO);
  auto const cmd_buf_info = vk::CommandBufferBeginInfo().setFlags(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
  auto result = swapchain_image_resources[index].graphics_to_present_cmd.begin(&cmd_buf_info);
  VERIFY(result == vk::Result::eSuccess);

  auto const image_ownership_barrier =
      vk::ImageMemoryBarrier()
          .setSrcAccessMask(vk::AccessFlags())
          .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite)
          .setOldLayout(vk::ImageLayout::ePresentSrcKHR)
          .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
          .setSrcQueueFamilyIndex(graphics_queue_family_index)
          .setDstQueueFamilyIndex(present_queue_family_index)
          .setImage(swapchain_image_resources[index].image)
          .setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1));

  swapchain_image_resources[index].graphics_to_present_cmd.pipelineBarrier(
      vk::PipelineStageFlagBits::eColorAttachmentOutput, vk::PipelineStageFlagBits::eColorAttachmentOutput,
      vk::DependencyFlagBits(), 0, nullptr, 0, nullptr, 1, &image_ownership_barrier);
  // TODO Investigate
  // result =
  swapchain_image_resources[index].graphics_to_present_cmd.end();
  //VERIFY(result == vk::Result::eSuccess);
}

void vlk::VulkanModule::updateDataBuffer(Camera *camera, vk::CommandBuffer &commandBuffer, GameObject *object) {
  FLOG(INFO) << "Object: " << object->getSid();
  mat4x4 VP;
  mat4x4_mul(VP, camera->getProjectionMatrix(), camera->getViewMatrix());

  // Rotate around the Y axis
  mat4x4 model{0};
  mat4x4_dup(model, object->getModelMatrix());
  mat4x4_rotate(object->getModelMatrix(), model, 0.0f, 1.0f, 0.0f,
                (float) degreesToRadians(object->getSpinningAngle()));

  mat4x4 MVP;
  mat4x4_mul(MVP, VP, object->getModelMatrix());

  auto data = device.mapMemory(swapchain_image_resources[current_buffer].uniform_memory,
                               0,
                               VK_WHOLE_SIZE,
                               vk::MemoryMapFlags());
  // TODO investigate API changes
//    VERIFY(data.result == vk::Result::eSuccess);

  memcpy(data, (const void *) &MVP[0][0], sizeof(MVP));

  device.unmapMemory(swapchain_image_resources[current_buffer].uniform_memory);
}

void vlk::VulkanModule::resetFenceAcquireNextImage() {
  FLOG(INFO);
  // Ensure no more than FRAME_LAG renderings are outstanding
  device.waitForFences(1, &fences[frame_index], VK_TRUE, UINT64_MAX);
  device.resetFences(1, &fences[frame_index]);

  vk::Result result;
  do {
    result = device.acquireNextImageKHR(swapchain, UINT64_MAX, image_acquired_semaphores[frame_index],
                                        vk::Fence(), &current_buffer);
    if (result == vk::Result::eErrorOutOfDateKHR) {
      // demo->swapchain is out of date (e.g. the window was resized) and
      // must be recreated:
      this->resize();
    } else if (result == vk::Result::eSuboptimalKHR) {
      // swapchain is not as optimal as it could be, but the platform's
      // presentation engine will still present the image correctly.
      break;
    } else {
      VERIFY(result == vk::Result::eSuccess);
    }
  } while (result != vk::Result::eSuccess);
}

void vlk::VulkanModule::draw(GameWorld *gameWorld) {
  static auto const start = vlk::TimeUtility::now();
  FLOG(INFO);
  this->resetFenceAcquireNextImage();
  this->drawWorld(gameWorld);

  this->presentFrame();
  curFrame++;
  if (curFrame % 100 == 0) {
    uint64_t milliSeconds = vlk::TimeUtility::now() - start;

    LOG(INFO) << "frame x second " << (curFrame * 1000) / (double) milliSeconds;
  }
}
// TODO Move this block outside of the vulkanmodule
void vlk::VulkanModule::drawWorld(vlk::GameWorld *gameWorld) {
  FLOG(INFO);
  auto gameObjects = gameWorld->getGameObjects();
  auto commandBufferCounts = gameObjects.size();
  if (commandBufferCounts > 0) {
    auto commandBuffers = this->prepareCommandBuffers(commandBufferCounts);

    for (int index = 0; index < commandBufferCounts; ++index) {
      this->updateDataBuffer(gameWorld->getCamera(), commandBuffers[index], gameObjects[index]);
    }
    vk::CommandBuffer mainCommandBuffer = this->prepareCommandBuffers(1)[0];

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.setPInheritanceInfo(nullptr);
    // TODO check this flag
    beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eRenderPassContinue);
    mainCommandBuffer.begin(beginInfo);
    mainCommandBuffer.executeCommands(commandBuffers);
    mainCommandBuffer.end();
  }
}

void vlk::VulkanModule::resize() {
  FLOG(INFO);
  uint32_t i;

  // Don't react to resize until after first initialization.
  if (!prepared) {
    return;
  }

  // In order to properly resize the window, we must re-create the
  // swapchain
  // AND redo the command buffers, etc.
  //
  // First, perform part of the cleanup() function:
  prepared = false;
//    auto result =
  device.waitIdle();
//    VERIFY(result == vk::Result::eSuccess);

  for (i = 0; i < swapchainImageCount; i++) {
    device.destroyFramebuffer(swapchain_image_resources[i].framebuffer, nullptr);
  }

  device.destroyDescriptorPool(desc_pool, nullptr);

  device.destroyPipeline(globalPipeline, nullptr);
  //device.destroyPipelineCache(pipelineCache, nullptr);
  device.destroyRenderPass(render_pass, nullptr);
  //device.destroyPipelineLayout(pipeline_layout, nullptr);
  device.destroyDescriptorSetLayout(desc_layout, nullptr);

  for (i = 0; i < this->textures.size(); i++) {
    device.destroyImageView(textures[i].view, nullptr);
    device.destroyImage(textures[i].image, nullptr);
    device.freeMemory(textures[i].mem, nullptr);
    device.destroySampler(textures[i].sampler, nullptr);
  }

  device.destroyImageView(depth.view, nullptr);
  device.destroyImage(depth.image, nullptr);
  device.freeMemory(depth.mem, nullptr);

  for (i = 0; i < swapchainImageCount; i++) {
    device.destroyImageView(swapchain_image_resources[i].view, nullptr);
    device.freeCommandBuffers(cmd_pool, 1, &swapchain_image_resources[i].cmd);
    device.freeCommandBuffers(cmd_pool,
                              (uint32_t) swapchain_image_resources[i].subCommands.size(),
                              swapchain_image_resources[i].subCommands.data());
    device.destroyBuffer(swapchain_image_resources[i].uniform_buffer, nullptr);
    device.freeMemory(swapchain_image_resources[i].uniform_memory, nullptr);
  }

  device.destroyCommandPool(cmd_pool, nullptr);
  if (separate_present_queue) {
    device.destroyCommandPool(present_cmd_pool, nullptr);
  }

  // Second, re-perform the prepare() function, which will re-create the
  // swapchain.
  engine->prepare();
}

void vlk::VulkanModule::prepareTexture(std::string &filename) {
  FLOG(INFO);
  this->prepareTexture(filename.c_str(), this->textureFormat);
}

vlk::ShaderModule *vlk::VulkanModule::getShaderModule() {
  return this->shaderModule;
}

vlk::VulkanPipelineModule *vlk::VulkanModule::getPipelineModule() {
  return this->pipelineModule;
}

void vlk::VulkanModule::presentFrame() {
  FLOG(INFO);
  // Wait for the image acquired semaphore to be signaled to ensure
  // that the image won't be rendered to until the presentation
  // engine has fully released ownership to the application, and it is
  // okay to render to the image.
  vk::PipelineStageFlags const pipe_stage_flags = vk::PipelineStageFlagBits::eColorAttachmentOutput;


  // clearCommandBuffer requires the render pass


  // TODO nothing is recorded inside the image presentation command buffer
  vk::CommandBuffer &imageCommandBuffer = swapchain_image_resources[current_buffer].cmd;
  this->clearBackgroundCommandBuffer(imageCommandBuffer,
                                     swapchain_image_resources[current_buffer].framebuffer);
  auto const submit_info = vk::SubmitInfo()
      .setPWaitDstStageMask(&pipe_stage_flags)
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(&image_acquired_semaphores[frame_index])
      .setCommandBufferCount(1)
      .setPCommandBuffers(&imageCommandBuffer)
      .setSignalSemaphoreCount(1)
      .setPSignalSemaphores(&draw_complete_semaphores[frame_index]);

  auto result = graphics_queue.submit(1, &submit_info, fences[frame_index]);
  VERIFY(result == vk::Result::eSuccess);

  if (separate_present_queue) {
    // If we are using separate queues, change image ownership to the
    // present queue before presenting, waiting for the draw complete
    // semaphore and signalling the ownership released semaphore when
    // finished
    auto const present_submit_info = vk::SubmitInfo()
        .setPWaitDstStageMask(&pipe_stage_flags)
        .setWaitSemaphoreCount(1)
        .setPWaitSemaphores(&draw_complete_semaphores[frame_index])
        .setCommandBufferCount(1)
        .setPCommandBuffers(&swapchain_image_resources[current_buffer].graphics_to_present_cmd)
        .setSignalSemaphoreCount(1)
        .setPSignalSemaphores(&image_ownership_semaphores[frame_index]);

    result = present_queue.submit(1, &present_submit_info, vk::Fence());
    VERIFY(result == vk::Result::eSuccess);
  }

  // If we are using separate queues we have to wait for image ownership,
  // otherwise wait for draw complete
  auto const presentInfo = vk::PresentInfoKHR()
      .setWaitSemaphoreCount(1)
      .setPWaitSemaphores(separate_present_queue ? &image_ownership_semaphores[frame_index]
                                                 : &draw_complete_semaphores[frame_index])
      .setSwapchainCount(1)
      .setPSwapchains(&swapchain)
      .setPImageIndices(&current_buffer);

  result = present_queue.presentKHR(&presentInfo);
  frame_index += 1;
  frame_index %= FRAME_LAG;
  if (result == vk::Result::eErrorOutOfDateKHR) {
    // swapchain is out of date (e.g. the window was resized) and
    // must be recreated:
    resize();
  } else if (result == vk::Result::eSuboptimalKHR) {
    // swapchain is not as optimal as it could be, but the platform's
    // presentation engine will still present the image correctly.
  } else {
    VERIFY(result == vk::Result::eSuccess);
  }
}
void vlk::VulkanModule::prepareRenderPassAndFramebuffer() {
  FLOG(INFO);
  this->prepareRenderPass();
  this->prepareFramebuffers();

}
void vlk::VulkanModule::preparePrimaryCommandBuffer(vk::CommandBuffer *commandBuffer) {
  FLOG(INFO);
  auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(this->cmd_pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(1);
  this->device.allocateCommandBuffers(&commandBufferAllocateInfo, commandBuffer);
}

std::vector<vk::CommandBuffer> vlk::VulkanModule::prepareCommandBuffers(uint32_t count) {
  FLOG(INFO);

  auto const commandBufferAllocateInfo = vk::CommandBufferAllocateInfo()
      .setCommandPool(this->cmd_pool)
      .setLevel(vk::CommandBufferLevel::ePrimary)
      .setCommandBufferCount(count);

  return this->device.allocateCommandBuffers(commandBufferAllocateInfo);
}
