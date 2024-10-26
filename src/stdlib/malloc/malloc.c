#include "malloc.h"
#include "config.h"


Block *freelist = NULL;
int allocated_blocks = 0;
int freed_blocks = 0;
Block *bins[BIN_COUNT] = {NULL};
Block *is_mmap = NULL;

void *_malloc(size_t size) {
    if (__builtin_expect(size == 0, 0))
        return NULL;
    size = ALIGN(size, ALIGNMENT);

    Block *block = NULL;
    if (size <= BIN_MAX_SIZE) {
        int bin_index = size / ALIGNMENT - 1;
        if (bin_index >= 0 && bin_index < BIN_COUNT && bins[bin_index]) {
            #if DEBUG
                printf("Allocating from bin %d\n", bin_index);
                printf("Block size: %zu\n", size);
                printf("\n");
            #endif
            Block *block = bins[bin_index];
            bins[bin_index] = block->next;
            block->free = 0;
            _memset_avx(block->aligned_address, 0, size);
            return block->aligned_address;
        }
    }
	printf("Allocating with malloc\n");
    if (size >= MMAP_THRESHOLD)
        return request_space_mmap(size, ALIGNMENT);
    else {
        if (!freelist) {
            block = request_space_sbrk(NULL, size, ALIGNMENT);
            if (__builtin_expect(!block, 0))
                return NULL;
            freelist = block;
        } else {
            Block *last = freelist;
            block = find_free_block(&last, size, ALIGNMENT);
            if (block) {
                if (block->size >= size + MBLOCK_SIZE)
                    split_block(block, size, ALIGNMENT);
                block->free = 0;
                _memset_avx(block->aligned_address, 0, size);
            } else {
                block = request_space_sbrk(last, size, ALIGNMENT);
                if (__builtin_expect(!block, 0))
                    return NULL;
            }
        }
    }

    return block->aligned_address;
}

void *_aligned_alloc(size_t alignment, size_t size) {
    if (__builtin_expect(size == 0, 0) || (alignment & (alignment - 1)) != 0)
        return NULL;
    
    size = ALIGN(size, alignment);
    Block *block = NULL;
    
    if (size <= CACHE_SIZE_L1) {
        _mm_prefetch(freelist, _MM_HINT_T0);
    } else if (size <= CACHE_SIZE_L2) {
        _mm_prefetch(freelist, _MM_HINT_T1);
    }
	#if DEBUG
		printf("Allocating %zu bytes with alignment %zu\n", size, alignment);
	#endif
    if (size >= MMAP_THRESHOLD) {
        return request_space_mmap(size, alignment);
    } else {
        if (!freelist) {
            block = request_space_sbrk(NULL, size, alignment);
            if (__builtin_expect(!block, 0))
                return NULL;
            freelist = block;
        } else {
            Block *last = freelist;
            block = find_free_block(&last, size, alignment);
            if (block) {
                if (block->size >= size + MBLOCK_SIZE)
                    split_block(block, size, alignment);
                block->free = 0;
                memset(block->aligned_address, 0, size); 
            } else {
                block = request_space_sbrk(last, size, alignment);
                if (__builtin_expect(!block, 0))
                    return NULL;
            }
        }
    }

    uintptr_t addr = (uintptr_t)block->aligned_address;
    uintptr_t aligned_addr = (addr + alignment - 1) & ~(alignment - 1);
    block->aligned_address = (void *)aligned_addr;

    if (alignment >= 32 ) 
		memset(block->aligned_address, 0, size);

    return block->aligned_address;
}


simpl_weak_alias(malloc, _malloc)
