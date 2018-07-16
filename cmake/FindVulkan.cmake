set(CMAKE_VERBOSE_MAKEFILE)
if (APPLE)
    set(VULKAN_BASE_DIR /Users/crypt/CLionProjects/Molten-0.18.2/MoltenVK)
    find_path(VULKAN_INCLUDE_DIR
            NAMES vulkan/vulkan.h
            HINTS ${VULKAN_BASE_DIR}/include/
            NO_DEFAULT_PATH)
    find_library(VULKAN_LIBRARY_DIR
            NAMES libMoltenVK.dylib
            HINTS ${VULKAN_BASE_DIR}/macOS
            NO_DEFAULT_PATH)

ELSEIF(LINUX)
    set(VULKAN_BASE_DIR /opt/VulkanSDK/1.0.54.0/)
    find_path(VULKAN_INCLUDE_DIR
            NAMES vulkan/vulkan.h
            HINTS ${VULKAN_BASE_DIR}/x86_64/include/
            NO_DEFAULT_PATH)
    find_library(VULKAN_LIBRARY_DIR
            NAMES lib/libvulkan.so
            HINTS ${VULKAN_BASE_DIR}/source
            NO_DEFAULT_PATH)

ENDIF()
message("VULKAN_BASE_DIR: " ${VULKAN_BASE_DIR})
message("VULKAN_INCLUDE_DIR: " ${VULKAN_INCLUDE_DIR})
message("VULKAN_LIBRARY_DIR: " ${VULKAN_LIBRARY_DIR})