//
// Created by crypt on 29/12/17.
//

#ifndef VULKAN_ENGINE_CPP_TIMERUTILITY_HPP
#define VULKAN_ENGINE_CPP_TIMERUTILITY_HPP
#include <cstdint>
namespace vlk {
  class TimerUtility {
   public:
    static uint64_t now();
  };
}
#endif //VULKAN_ENGINE_CPP_TIMERUTILITY_HPP
