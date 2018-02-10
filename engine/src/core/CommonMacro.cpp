#include <iomanip>
#include "core/CommonMacro.hpp"



std::string LogIndent::separators[] = { "", "--", "----", "------", "        ", "          "};

std::atomic<int> LogIndent::level(0);

std::ostream&
operator<<(std::ostream& oStream, const LogIndent &logIndent) {
  int currentLevel = LogIndent::level.load();
  if (currentLevel > 0) {
    oStream << std::setw(2) << currentLevel << std::setw(currentLevel*2) << std::setfill('-') << "";
  }
  return oStream;
}
