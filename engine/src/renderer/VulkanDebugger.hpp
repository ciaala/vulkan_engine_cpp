//
// Created by crypt on 12/03/18.
//

#ifndef VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
#define VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan.h"
namespace vlk {
class VulkanDebugger {
  // PRIVATE STATIC FIELD
  private:
    static int SKIP_VULKAN_DEBUGGER_FUNCTION_FRAMES;

  // PRIVATE STATIC FUNCTION
  static VkBool32 vkDebugCallback(
      VkDebugReportFlagsEXT flags,
      VkDebugReportObjectTypeEXT objType,
      uint64_t srcObj,
      size_t location,
      int32_t msgCode,
      const char *layerPrefix,
      const char *msg,
      void *userData);

  // PRIVATE FIELD
 private:
  PFN_vkCreateDebugReportCallbackEXT pfnCreateDebugReportCallbackEXT;
  PFN_vkDestroyDebugReportCallbackEXT pfnDestroyDebugReportCallbackEXT;
  vk::Instance &instance;

  VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo;
  VkDebugReportCallbackEXT debugReportCallback;

  // PRIVATE METHOD
 private:

  void init_debug_callback();
  bool debugCallBack(
      VkDebugReportFlagsEXT flags,
      VkDebugReportObjectTypeEXT objectType,
      uint64_t sourceObject,
      size_t location,
      int32_t messageCode,
      const char *layerPrefix,
      const char *message);
  void print_backtrace();

  // PUBLIC METHOD
 public :
  explicit VulkanDebugger(vk::Instance &instance);
  ~VulkanDebugger();

};

}
#endif //VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
