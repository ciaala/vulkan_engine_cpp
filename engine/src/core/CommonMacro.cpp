#include <iomanip>
#include "core/CommonMacro.hpp"

std::atomic_uint_fast32_t LogIndent::level(0);

std::ostream&
operator<<(std::ostream& oStream, const LogIndent &logIndent) {
  uint_fast32_t currentLevel = LogIndent::level.load();
  if (currentLevel > 0) {
    oStream << std::setw(2)
            << currentLevel
            << std::setw(1+currentLevel*2)
            << std::setfill('-') ;
  }
  return oStream;
}
