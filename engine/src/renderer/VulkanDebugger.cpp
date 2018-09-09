//
// Created by crypt on 12/03/18.
//
#include <vulkan/vulkan.h>
#include <execinfo.h>
#include <cxxabi.h>
#include "core/CommonMacro.hpp"
#include "VulkanDebugger.hpp"
#include "iostream"
using namespace std;


int vlk::VulkanDebugger::SKIP_VULKAN_DEBUGGER_FUNCTION_FRAMES = 3;

vlk::VulkanDebugger::VulkanDebugger(vk::Instance &instance) :
    instance(instance) {
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
  auto *vulkanDebugger = reinterpret_cast<VulkanDebugger *>(userData);
  return vulkanDebugger->debugCallBack(flags, objType, srcObj, location, msgCode, layerPrefix, msg) ? VK_TRUE
                                                                                                    : VK_FALSE;

}
bool vlk::VulkanDebugger::debugCallBack(
    // TODO(ciaala) extend to print line number also
    // - http://man7.org/linux/man-pages/man3/backtrace.3.html
    // - also check https://stackoverflow.com/questions/77005/how-to-automatically-generate-a-stacktrace-when-my-gcc-c-program-crashes
    // + library which should support already reading dwarf file: https://github.com/ianlancetaylor/libbacktrace
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
      print_backtrace();
    }
    buffer << endl;

    if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
      LOG(ERROR) << buffer.str();
    } else if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
      LOG(WARNING) << buffer.str();
    } else if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
      LOG(INFO) << buffer.str();
    } else if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
      LOG(INFO) << buffer.str();
    }
  }

  return false;

}

void
print_function_name(char *function_definition, std::stringstream &buffer) {

  char *begin_name = nullptr, *begin_offset = nullptr, *end_offset = nullptr;

  for (char *p = function_definition; *p; ++p) {
    if (*p == '(')
      begin_name = p;
    else if (*p == '+')
      begin_offset = p;
    else if (*p == ')' && begin_offset) {
      end_offset = p;
      break;
    }
  }

  if (begin_name && begin_offset && end_offset
      && begin_name < begin_offset) {
    *begin_name++ = '\0';
    *begin_offset++ = '\0';
    *end_offset = '\0';

    // mangled name is now in [begin_name, begin_offset) and caller
    // offset in [begin_offset, end_offset). now apply
    // __cxa_demangle():

    int status = 0;
    if (begin_name != nullptr) {
      size_t funcnamesize = 1024;
      char *funcname = (char *) malloc(funcnamesize);
      char *ret = abi::__cxa_demangle(begin_name,
                                      funcname,
                                      &funcnamesize,
                                      &status);
      if (status == 0) {
        buffer << "\033[4m" << ret << "[" <<begin_offset << "]\033[0m";
      } else {
        buffer << "\033[9m" << function_definition << "[" <<  begin_offset << "]\033[0m";
      }
      free(funcname);
      return;
    }
  }
  buffer << function_definition;
}

void
vlk::VulkanDebugger::print_backtrace() {
  static const auto BACKTRACE_BUFFER_SIZE = 512;

  int j, nptrs;
  void *buffer[BACKTRACE_BUFFER_SIZE];
  char **strings;

  nptrs = backtrace(buffer, BACKTRACE_BUFFER_SIZE);
  // printf("backtrace() returned %d addresses\n", nptrs);

  /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
     would produce similar output to the following: */

  strings = backtrace_symbols(buffer, nptrs);
  if (strings == nullptr) {
    LOG(ERROR) << "ERROR in backtrace_symbols : " << strerror(errno);
    exit(EXIT_FAILURE);
  }

  std::stringstream stacktrace;
  for (j = SKIP_VULKAN_DEBUGGER_FUNCTION_FRAMES; j < nptrs; j++) {
    stacktrace << "\t\t" ;
    print_function_name(strings[j], stacktrace);
    stacktrace << std::endl;
  }
  std::cerr << stacktrace.str();
  free(strings);
}

