set(CMAKE_VERBOSE_MAKEFILE)
set(VULKAN_BASE_DIR /opt/VulkanSDK/current/)
message("VULKAN_BASE_DIR: " ${VULKAN_BASE_DIR})
find_path(VULKAN_INCLUDE_DIR
        NAMES vulkan/vulkan.h
        HINTS ${VULKAN_BASE_DIR}/x86_64/include/
        NO_DEFAULT_PATH)
find_library(VULKAN_LIBRARY_DIR
        NAMES lib/libvulkan.so
        HINTS ${VULKAN_BASE_DIR}/source
        NO_DEFAULT_PATH)
message("VULKAN_INCLUDE_DIR: " ${VULKAN_INCLUDE_DIR})
message("VULKAN_LIBRARY_DIR: " ${VULKAN_LIBRARY_DIR})