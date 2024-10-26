#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "func_ifunc_selector.h"
#include <sys/stdargs.h>

#if defined(__APPLE__)
#define simpl_weak_alias(func, func_source) \
    __typeof__(func_source) __attribute__((weak)) *func = &func_source;
#elif defined(__GNUC__) || defined(__clang__)
#define simpl_weak_alias(func, func_source) \
    extern __typeof__(func_source) func __attribute__((weak, alias(#func_source)));
#else
#define simpl_weak_alias(name, func)                                           \
  _Pragma("GCC error \"Weak alias is not supported on this compiler\"")
#endif

#define AVX2_ALIGNMENT 32
#define AVX2_SIZE 32
#define BLOCK_SIZE 4096
#define CHAR_PER_VEC (AVX2_SIZE / sizeof(char))
#define VEC_SIZE 32

#endif //__CONFIG_H__

