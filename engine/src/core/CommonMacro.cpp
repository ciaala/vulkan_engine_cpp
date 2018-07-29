#include <iomanip>
#include "core/CommonMacro.hpp"

std::atomic_uint_fast32_t LogIndent::level(0);

std::ostream&
operator<<(std::ostream& oStream, const LogIndent &logIndent) {
  uint_fast32_t currentLevel = LogIndent::level.load();
  if (currentLevel > 0) {
    oStream << std::setw(2)
            << currentLevel
            << std::setw(1 + currentLevel * 2)
            << std::setfill('-') ;
  }
  return oStream;
}

LogIndent::~LogIndent() {

  uint_fast32_t level = LogIndent::level.fetch_sub(1);
/*  int width = 2* ((int) level);
  google::LogMessage logMessage(filename.data(), line, severity);
  logMessage.stream() << std::setw(2) << level << std::setw(width) << std::setfill('-') << "";
*/
 }

LogIndent::LogIndent(
    const std::string filename,
    const int line,
    const int severity,
    const std::string function) :
    function(function),
    filename(filename),
    line(line),
    severity(severity)
{
  LogIndent::level.fetch_add(1);
}
