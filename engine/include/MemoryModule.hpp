//
// Created by crypt on 17/07/17.
//

#ifndef VULKAN_ENGINE_CPP_MEMORYMODULE_HPP
#define VULKAN_ENGINE_CPP_MEMORYMODULE_HPP

#include <cstdint>
#include <vulkan/vulkan.hpp>

namespace vlk {
    class MemoryModule {
    public:
        MemoryModule( vk::PhysicalDeviceMemoryProperties *memory_properties);
        bool memoryTypeFromProperties(uint32_t typeBits, vk::MemoryPropertyFlags requirements_mask, uint32_t *typeIndex);

        vk::PhysicalDeviceMemoryProperties *memory_properties;
    };

}

#endif //VULKAN_ENGINE_CPP_MEMORYMODULE_HPP
