/*
 *  this implementation of the strlen function using SSE instructions
 *  less instruction, less cache misses, less branch mispredictions
 *  this implementation is faster than the first one but handle less cases
 *  Explanation:
 *  The function uses SSE instructions to find the length of a string.
 *  The function loads 16 bytes at a time and checks if there is a zero byte in the chunk.
 *  If there is a zero byte, it returns the position of the zero byte in the 16 bytes chunk.
 *  If there is no zero byte, it increments the pointer by 16 bytes and repeats the process.
 */

#include <config.h>
#include <immintrin.h>
#include <stdint.h>
#include <stdlib.h>

static inline size_t _strlen_sse(const char *str)
{
    if (!str) return 0;

    const char *start = str;
    __m128i zero = _mm_set1_epi8(0);
    uintptr_t addr = (uintptr_t)str;
    size_t mis = addr & 15;

    if (mis != 0)
    {
        size_t partial = 16 - mis;
        __m128i data = _mm_loadu_si128((const __m128i*)str);
        __m128i cmp  = _mm_cmpeq_epi8(zero, data);
        int mask     = _mm_movemask_epi8(cmp);

        if (mask != 0)
        {
            int idx = __builtin_ctz(mask);
            return (size_t)(str + idx - start);
        }
        str += partial;
    }

    for (;;)
    {
        __m128i data = _mm_load_si128((const __m128i*)str);
        __m128i cmp  = _mm_cmpeq_epi8(zero, data);
        int mask     = _mm_movemask_epi8(cmp);

        if (mask != 0)
        {
            int idx = __builtin_ctz(mask);
            return (size_t)(str + idx - start);
        }
        str += 16;
    }
    return 0;
}
