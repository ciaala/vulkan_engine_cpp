//
// Created by crypt on 12/03/18.
//
#include <vulkan/vulkan.h>
#include <jmorecfg.h>
#include "core/CommonMacro.hpp"
#include "VulkanDebugger.hpp"
#include "iostream"
#include "sstream"
using namespace std;
vlk::VulkanDebugger::VulkanDebugger(vk::Instance &instance) :
instance(instance){
  init_debug_callback();
}
void
vlk::VulkanDebugger::init_debug_callback() {
  FLOG(INFO);

  this->pfnCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(
      vkGetInstanceProcAddr(this->instance, "vkCreateDebugReportCallbackEXT"));
  this->pfnDestroyDebugReportCallbackEXT = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(
      vkGetInstanceProcAddr(this->instance, "vkDestroyDebugReportCallbackEXT"));
  if (this->pfnCreateDebugReportCallbackEXT && this->pfnDestroyDebugReportCallbackEXT) {
    LOG(INFO) << "[VulkanDebugger]: initialization completed !" << endl;
  } else {
    LOG(FATAL) << "[VulkanDebugger]: initialization failed !" << endl;
    exit(0);
  }
  debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
  debugReportCreateInfo.flags =
      VK_DEBUG_REPORT_ERROR_BIT_EXT
          | VK_DEBUG_REPORT_WARNING_BIT_EXT
          | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT
          | VK_DEBUG_REPORT_DEBUG_BIT_EXT
          | VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
  debugReportCreateInfo.pfnCallback = vkDebugCallback;
  debugReportCreateInfo.pUserData = this;
  pfnCreateDebugReportCallbackEXT(this->instance, &this->debugReportCreateInfo, nullptr, &this->debugReportCallback);
}

vlk::VulkanDebugger::~VulkanDebugger() {
  this->pfnDestroyDebugReportCallbackEXT(this->instance, this->debugReportCallback, nullptr);
  debugReportCallback = VK_NULL_HANDLE;
}
VkBool32 vlk::VulkanDebugger::vkDebugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t srcObj,
    size_t location,
    int32_t msgCode,
    const char *layerPrefix,
    const char *msg,
    void *userData) {
  auto *vulkanDebugger = reinterpret_cast<VulkanDebugger*>(userData);
  return vulkanDebugger->debugCallBack(flags, objType, srcObj, location, msgCode, layerPrefix, msg) ? VK_TRUE : VK_FALSE;

}
bool vlk::VulkanDebugger::debugCallBack(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objectType,
    uint64_t sourceObject,
    size_t location,
    int32_t messageCode,
    const char *layerPrefix,
    const char *message) {


    if (flags != VK_DEBUG_REPORT_INFORMATION_BIT_EXT &&
        flags != VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
      std::stringstream buffer;

      if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
        buffer << "[I| ";
      } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
        buffer << "[W| ";
      } else if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
        buffer << "[P| ";
      } else if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        //std::cout << "\033[1;31mbold [ERROR ***** | ";
        buffer << "\033[0;1;32;4m[E| ";
      } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
        buffer << "[D| ";
      }

      buffer << '(' << objectType << ")" << layerPrefix << "] " << message;
      if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
        buffer << "\033[0m";
      }
      buffer << endl;
      LOG(INFO) << buffer.str();
    }

    return false;

}
