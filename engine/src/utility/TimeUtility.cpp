//
// Created by crypt on 29/12/17.
//
#include <time.h>


#include "TimeUtility.hpp"
uint64_t vlk::TimeUtility::now() {
  timespec ts{0};
  timespec_get(&ts, TIME_UTC);
  uint64_t milliseconds = ((uint64_t) ts.tv_sec) * 1000;
  milliseconds += ts.tv_nsec / (1000*1000);
  return milliseconds;
}
