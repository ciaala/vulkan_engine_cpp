//
// Created by crypt on 17/07/17.
//
#include "gflags/gflags.h"
#include "glog/logging.h"

#ifndef VULKAN_ENGINE_CPP_COMMONMACRO_HPP
#define VULKAN_ENGINE_CPP_COMMONMACRO_HPP
#define ERR_EXIT(err_msg, err_class) \
    do {                             \
        printf("%s\n", err_msg);     \
        fflush(stdout);              \
        exit(1);                     \
    } while (0)

#define VERIFY(x) assert(x)
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

#endif //VULKAN_ENGINE_CPP_COMMONMACRO_HPP
