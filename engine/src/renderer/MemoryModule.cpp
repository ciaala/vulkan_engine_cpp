//
// Created by crypt on 17/07/17.
//

#include "renderer/MemoryModule.hpp"

bool vlk::MemoryModule::memoryTypeFromProperties(uint32_t typeBits, vk::MemoryPropertyFlags requirements_mask,
                                                 uint32_t *typeIndex) {
    // Search memtypes to find first index with those properties
    for (uint32_t i = 0; i < VK_MAX_MEMORY_TYPES; i++) {
        if ((typeBits & 1) == 1) {
            // Type is available, does it match user properties?
            if ((memory_properties->memoryTypes[i].propertyFlags & requirements_mask) == requirements_mask) {
                *typeIndex = i;
                return true;
            }
        }
        typeBits >>= 1;
    }

    // No memory types matched, return failure
    return false;
}

vlk::MemoryModule::MemoryModule(vk::PhysicalDeviceMemoryProperties *memory_properties) {
    this->memory_properties = memory_properties;

}
