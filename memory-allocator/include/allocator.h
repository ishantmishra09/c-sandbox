#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stddef.h>
#include <stdint.h>

#define ALIGNMENT 16

typedef struct block {
  size_t size;
  struct block *next;
  int free;
  uint32_t padding;
} block_t;

#define BLOCK_SIZE (((sizeof(block_t) + ALIGNMENT - 1) / ALIGNMENT) * ALIGNMENT)

block_t *find_free_block(size_t size);
block_t *request_space(size_t size);
void split_block(block_t *block, size_t size);
void coalesce(void);

void *zmalloc(size_t size);
void zfree(void *ptr);
void *zcalloc(size_t nmemb, size_t size);
void *zrealloc(void *ptr, size_t size);

#endif
