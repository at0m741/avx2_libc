#include <config.h>

#define strlen _strlen
#include <string.h>
#undef strlen

simpl_func_ifunc_init(_strlen, size_t, IFUNC_AVX | IFUNC_SSE, const char *);

simpl_weak_alias(strlen, _strlen)
