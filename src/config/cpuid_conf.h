#ifndef __CPUID_CONF_H__
#define __CPUID_CONF_H__

#include <stdint.h>

#define bit_MMX        (1 << 23)
#define bit_SSE        (1 << 25)
#define bit_SSE2       (1 << 26)
#define bit_SSE3       (1 << 0)
#define bit_SSSE3      (1 << 9)
#define bit_SSE4_1     (1 << 19)
#define bit_SSE4_2     (1 << 20)
#define bit_AVX        (1 << 28)
#define bit_AVX2       (1 << 5)
#define bit_AVX512F    (1 << 16)
#define bit_AVX512DQ   (1 << 17)
#define bit_AVX512IFMA (1 << 21)
#define bit_AVX512PF   (1 << 26)
#define bit_AVX512ER   (1 << 27)
#define bit_AVX512CD   (1 << 28)
#define bit_AVX512BW   (1 << 30)
#define bit_AVX512VL   (1 << 31)

typedef union simd_support_u {
    uint32_t cpu_id;
    struct {
        uint8_t mmx       : 1;
        uint8_t sse       : 1;
        uint8_t sse2      : 1;
        uint8_t sse3      : 1;
        uint8_t ssse3     : 1;
        uint8_t sse41     : 1;
        uint8_t sse42     : 1;
        uint8_t avx       : 1;
        uint8_t avx2      : 1;
        uint8_t avx512f   : 1;
        uint8_t avx512dq  : 1;
        uint8_t avx512ifma: 1;
        uint8_t avx512pf  : 1;
        uint8_t avx512er  : 1;
        uint8_t avx512cd  : 1;
        uint8_t avx512bw  : 1;
        uint8_t avx512vl  : 1;
        uint8_t erms      : 1;
        uint32_t padding  : 14;
    };
} simd_support_t;

void __attribute__((constructor)) init_simd_support(void);

extern simd_support_t simd_support;

#endif // __CPUID_CONF_H__
