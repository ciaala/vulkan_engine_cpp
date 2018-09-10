//
// Created by crypt on 10/09/18.
//

#ifndef VULKAN_ENGINE_CPP_SHUFFLINGCOLOR_H
#define VULKAN_ENGINE_CPP_SHUFFLINGCOLOR_H

#include <chrono>
#include <random>

namespace vlk {
class ShufflingColor {
  // PROPERTIES
 public:
  uint32_t red = 1;
  uint32_t green = 1;
  uint32_t blue = 1;

  //
 private:
  double coeffRed = 0;
  double coeffGreen = 0;
  double coeffBlue = 0;

  uint32_t COLOR_RANGE = 512;
  uint32_t TIMELIMIT_MILLIS = 5000;
  std::chrono::milliseconds TIMEFRAME_LIMIT = std::chrono::milliseconds(5000);
  double MAXCOEFF_MILLI = (double) COLOR_RANGE / (double) TIMELIMIT_MILLIS;
  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds>
      sequenceEnd = std::chrono::_V2::system_clock::now();
  std::chrono::time_point<std::chrono::_V2::system_clock, std::chrono::nanoseconds>
      previousTime = std::chrono::_V2::system_clock::now();

  std::default_random_engine generator;

  std::uniform_real_distribution<> distribution =
      std::uniform_real_distribution<>(0.0, MAXCOEFF_MILLI * 2);

  // PRIVATE METHOD
  void shuffleCoeffs();
  // PUBLIC METHOD
 public:
  void next();
};
}

#endif //VULKAN_ENGINE_CPP_SHUFFLINGCOLOR_H
