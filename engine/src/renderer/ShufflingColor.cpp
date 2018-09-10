//
// Created by crypt on 10/09/18.
//

#include <iostream>
#include <functional>
#include "ShufflingColor.hpp"
#include "core/CommonMacro.hpp"

void vlk::ShufflingColor::shuffleCoeffs() {
  auto dice = bind(distribution, generator);
  this->coeffBlue = dice();
  this->coeffGreen = dice();
  this->coeffRed = MAXCOEFF_MILLI * 2 - coeffGreen - coeffBlue;
  this->coeffRed = this->coeffRed < 0 ? 0 : this->coeffRed;
  FLOG(ERROR) << "COEFF: [" << coeffRed << "," << coeffGreen << "," << coeffBlue << "] " << MAXCOEFF_MILLI;

}

void vlk::ShufflingColor::next() {
  const std::chrono::time_point now = std::chrono::_V2::system_clock::now();
  if (sequenceEnd < now) {
    this->shuffleCoeffs();
    sequenceEnd = now + TIMEFRAME_LIMIT;
  }

  auto delta_MILLIS = std::chrono::duration_cast<std::chrono::milliseconds>
      (now - previousTime)
      .count();
  blue += (uint32_t) ((coeffBlue * delta_MILLIS) * 1000);
  green += (uint32_t) ((coeffGreen * delta_MILLIS) * 1000);
  red += (uint32_t) ((coeffRed * delta_MILLIS) * 1000);
  red %= 255000;
  green %= 255000;
  blue %= 255000;
  previousTime = now;
  FLOG(INFO) << "COLOR: [" << red << "," << green << "," << blue << "] ~" << delta_MILLIS;
}
