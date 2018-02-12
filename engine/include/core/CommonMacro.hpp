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
#ifndef VULKAN_ENGINE_CPP_COMMONMACRO_HPP
#define VULKAN_ENGINE_CPP_COMMONMACRO_HPP

class LogIndent {
  static std::atomic_int level;
  static std::string separators[];
 public:
  LogIndent() {
    LogIndent::level.fetch_add(1);
  }

  ~LogIndent() {
    LogIndent::level.fetch_sub(1);
  }
  friend std::ostream &operator<<(std::ostream &oStream, const LogIndent &logIndent);

};

#define FLOG(severity) \
  LogIndent __logIndent; \
  LOG(severity) << __logIndent << "<" << __PRETTY_FUNCTION__ << "> "

#define ERR_EXIT(err_msg, err_class) \
    do {                             \
        printf("%s\n", err_msg);     \
        fflush(stdout);              \
        exit(1);                     \
    } while (0)

#define VERIFY(x) assert(x)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif //VULKAN_ENGINE_CPP_COMMONMACRO_HPP
