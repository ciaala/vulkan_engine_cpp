//
// Created by crypt on 17/07/17.
//
#include <vector>
#include <string>
#include "gflags/gflags.h"
#include "glog/logging.h"
#include <ostream>
#include <ogg/ogg.h>
#include <atomic>
#include <glog/logging.h>
#include <iomanip>
#ifndef VULKAN_ENGINE_CPP_COMMONMACRO_HPP
#define VULKAN_ENGINE_CPP_COMMONMACRO_HPP

#define GLOG_SEVERITY_INFO 0
#define GLOG_SEVERITY_WARNING 1
#define GLOG_SEVERITY_ERROR 2
#define GLOG_SEVERITY_FATAL 3


class LogIndent {

 private:

  static std::atomic_uint_fast32_t level;
  const std::string filename;
  const int line;
  const std::string function;
  const int severity;

 public:
  LogIndent(const std::string filename,
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

  ~LogIndent() {
    uint_fast32_t level = LogIndent::level.fetch_sub(1);
    int width = 2* ((int) level);
    google::LogMessage logMessage(filename.data(), 5555, severity);
    logMessage.stream() << std::setw(2) << level << std::setw(width) << std::setfill('-')<<"";
  }
  friend std::ostream &operator<<(std::ostream &oStream, const LogIndent &logIndent);

};

#define FLOG(severity) \
  LogIndent __logIndent(__FILE__, __LINE__, GLOG_SEVERITY_ ## severity, __PRETTY_FUNCTION__); \
  LOG(severity) << __logIndent << "<" << __PRETTY_FUNCTION__ << "> "

#define ERR_EXIT(err_msg, err_class) \
    do {                             \
        printf("%s\n", err_msg);     \
        fflush(stdout);              \
        exit(1);                     \
    } while (0)

#define VERIFY(x) assert(x)
#define CHECK_RESULT(x,y) \
if((x) != (y)) { \
  LOG(ERROR) << "FAILED " << std::dec << (x) << " == " << std::dec << (y); \
  assert((x) == (y)); \
}
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif //VULKAN_ENGINE_CPP_COMMONMACRO_HPP
