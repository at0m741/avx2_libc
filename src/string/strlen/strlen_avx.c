
/* check if the vector v contains a zero byte */
/* return 0 if no zero byte is found, otherwise return a non-zero value */

/*
** trying to benchmark Glibc.... but skill issue atm

* using _mm256_setzero_si256() to create a vector of 32 zero bytes bc assingning a 0 is to lower
(yes I know it's a bit overkill)
* using _mm256_loadu_si256() to load 32 bytes from the memory pointed by ptr in asm :
*                      vpcmpeqb ymm1, ymm0, ymmword ptr [rsi + rcx + 32]

* using has_zero_byte() to check if the vector v contains a zero byte
__builtin_ctz() is a builtin function that returns the number of trailing 0-bits
*/

#include <config.h>
#include <immintrin.h>
#include <stdlib.h>

static inline size_t _strlen_avx(const char *str)
{
    if (!str) return 0;

    const char *start = str;
    __m256i zero = _mm256_set1_epi8(0);
    uintptr_t addr = (uintptr_t)str;
    size_t mis = addr & 31;

    if (mis != 0)
    {
        size_t partial = 32 - mis; 
        __m256i data = _mm256_loadu_si256((const __m256i*)str);
        __m256i cmp  = _mm256_cmpeq_epi8(zero, data);
        int mask     = _mm256_movemask_epi8(cmp);

        if (mask != 0)
        {
            int idx = __builtin_ctz(mask);
            return (size_t)(str + idx - start);
        }
        str += partial;  
    }

    for (;;)
    {
        __m256i data = _mm256_load_si256((const __m256i*)str);
        __m256i cmp  = _mm256_cmpeq_epi8(zero, data);
        int mask     = _mm256_movemask_epi8(cmp);

        if (mask != 0)
        {
            int idx = __builtin_ctz(mask);
            return (size_t)(str + idx - start);
        }
        str += 32;
    }
    return 0;
}
