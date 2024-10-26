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

size_t _strlen_sse(const char *s)
{
    if (__builtin_expect(s == NULL, 0))
        return 0;

    const char *ptr = s;
    __m128i     zero = _mm_setzero_si128();
    size_t      misalignment = (size_t)s & 15;
    /*
     * If the pointer is not aligned on a 16 bytes boundary,
     * the function handles the initial bytes separately.
     * It loads 16 bytes from the memory pointed by ptr and checks if there is a zero byte in the
     * chunk.
     */
    if (misalignment)
    {
        __m128i  data = _mm_loadu_si128((const __m128i *)s);
        __m128i  cmp = _mm_cmpeq_epi8(data, zero);
        uint32_t mask = _mm_movemask_epi8(cmp);

        if (mask)
            return (s - ptr) + __builtin_ctz(mask);
        s += 16 - misalignment;
    }
    /*
     * Prefetch the next 32 bytes to improve performance.
     * The _MM_HINT_NTA hint is used to indicate that the data is not accessed again soon.
     * This hint is useful when the data is not accessed sequentially.
     * The _MM_HINT_T0 hint is used to indicate that the data is accessed soon.
     * This hint is useful when the data is accessed sequentially.
     */
    while (1)
    {
        __m128i  data = _mm_loadu_si128((const __m128i *)s);
        __m128i  cmp = _mm_cmpeq_epi8(data, zero);
        uint32_t mask = _mm_movemask_epi8(cmp);
        if (mask)
            return (s - ptr) + __builtin_ctz(mask);
        s += 16;
    }
}
