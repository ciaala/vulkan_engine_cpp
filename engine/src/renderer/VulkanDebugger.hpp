//
// Created by crypt on 12/03/18.
//

#ifndef VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
#define VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan.h"
namespace vlk {
class VulkanDebugger {
 private:
  void init_debug_callback();
  PFN_vkCreateDebugReportCallbackEXT pfnCreateDebugReportCallbackEXT;
  PFN_vkDestroyDebugReportCallbackEXT pfnDestroyDebugReportCallbackEXT;
  vk::Instance &instance;

  VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo;
  VkDebugReportCallbackEXT debugReportCallback;

  static VkBool32 vkDebugCallback(
      VkDebugReportFlagsEXT flags,
      VkDebugReportObjectTypeEXT objType,
      uint64_t srcObj,
      size_t location,
      int32_t msgCode,
      const char *layerPrefix,
      const char *msg,
      void *userData);
  bool debugCallBack(
      VkDebugReportFlagsEXT flags,
      VkDebugReportObjectTypeEXT objectType,
      uint64_t sourceObject,
      size_t location,
      int32_t messageCode,
      const char *layerPrefix,
      const char *message);
 public :
  explicit VulkanDebugger(vk::Instance &instance);
  ~VulkanDebugger();

};

}
#endif //VULKAN_ENGINE_CPP_VULKANDEBUGGER_HXX
