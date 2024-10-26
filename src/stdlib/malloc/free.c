
#include "malloc.h"
#include "config.h"

extern Block *freelist;
extern size_t block_size;
extern int allocated_blocks;
extern int freed_blocks;

/*
	* Function to coalesce free blocks
	* this function is called after freeing a block
	* it coalesces adjacent free blocks
	* if two adjacent blocks are free, they are merged into a single block
	* this is done to reduce fragmentation
	* the size of the first block is increased by the size of the second block
	* the next pointer of the first block is updated to point to the block after the second block
	* this process is repeated until no more free blocks can be coalesced
	* this function is called after freeing a block
*/

#define MAX_BLOCK_SIZE 1024 * 1024
void coalesce_free_blocks() {
    Block *current = freelist;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += MBLOCK_SIZE + current->next->size;
            current->next = current->next->next;

            uintptr_t aligned_addr = (uintptr_t)(current + 1);
            if (aligned_addr % ALIGNMENT != 0) 
                printf("Warning: Coalesced block not aligned at %p\n", (void *)aligned_addr);
            #ifdef DEBUG
                printf("Coalesced block at %p with block at %p\n", current, current->next);
                printf("New block size: %zu\n", current->size);
                printf("\n");
            #endif
        } else {
            current = current->next;
			printf("current->size = %zu\n", current->size);
        }
    }
}

/*
	* Function to free a block of memory
	* ptr: pointer to the block to be freed
	* this function is called to free a block of memory
	* if the block was allocated using mmap, it is freed using munmap
	* if the block was allocated using sbrk, it is marked as free
	* the block is added to the freelist
	* the free flag is set to 1
	* the number of freed blocks is incremented
*/


void _free(void *ptr) {
    if (__builtin_expect(ptr == NULL, 0))
        return;

    Block *block = (Block *)((uintptr_t)ptr - sizeof(Block));
    assert(block != NULL);
    assert(block->aligned_address == ptr);
    assert(block->size > 0);

    printf("block->size = %zu\n", block->size);
    if (block->is_mmap) {
        _munmap((void *)block, block->size + sizeof(Block));
        freed_blocks++;
        #ifdef DEBUG
            printf("Freeing mmap block at %p\n", block);
            printf("Allocated blocks: %d\n", allocated_blocks);
            printf("Size of block: %zu\n", block->size);
            printf("\n");
        #endif
    } else {
        block->free = 1;
        block->is_mmap = 0;
        freed_blocks++;
        #ifdef DEBUG
            printf("free then coalesce_free_blocks\n");
            printf("Block at %p has size %zu\n", block, block->size);
            printf("Freeing block at %p\n", block);
			printf("freed_blocks = %d\n", freed_blocks);
            printf("\n");
        #endif
        coalesce_free_blocks();
    }
}


simpl_weak_alias(free, _free)
