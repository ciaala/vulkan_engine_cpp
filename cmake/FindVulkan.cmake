set(CMAKE_VERBOSE_MAKEFILE)
set(VULKAN_BASE_DIR /opt/VulkanSDK/1.0.54.0/x86_64)
message("VULKAN_BASE_DIR: " ${VULKAN_BASE_DIR})
find_path(VULKAN_INCLUDE_DIR
        NAMES vulkan/vulkan.h
        HINTS ${VULKAN_BASE_DIR}/include/
        NO_DEFAULT_PATH)
find_library(VULKAN_LIBRARY_DIR
        NAMES lib/libvulkan.so
        HINTS ${VULKAN_BASE_DIR}
        NO_DEFAULT_PATH)
message("VULKAN_INCLUDE_DIR: " ${VULKAN_INCLUDE_DIR})
message("VULKAN_LIBRARY_DIR: " ${VULKAN_LIBRARY_DIR})