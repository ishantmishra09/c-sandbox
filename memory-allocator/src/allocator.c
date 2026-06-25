#define _GNU_SOURCE

#include "../include/allocator.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static block_t *head = NULL;

static size_t align_size(size_t size) {
  if (size > SIZE_MAX - (ALIGNMENT - 1))
    return 0;

  return (size + ALIGNMENT - 1) & ~(ALIGNMENT - 1);
}

block_t *find_free_block(size_t size) {
  block_t *current = head;

  while (current) {
    if (current->free && current->size >= size)
      return current;

    current = current->next;
  }

  return NULL;
}

block_t *request_space(size_t size) {
  size = align_size(size);

  void *req = sbrk(size + BLOCK_SIZE);

  if (req == (void *)-1)
    return NULL;

  block_t *block = (block_t *)req;

  block->size = size;
  block->free = 0;
  block->next = NULL;

  return block;
}

void split_block(block_t *block, size_t size) {
  size = align_size(size);

  if (block->size < size + BLOCK_SIZE + ALIGNMENT)
    return;

  block_t *new_block = (block_t *)((char *)(block + 1) + size);

  new_block->size = block->size - size - BLOCK_SIZE;
  new_block->free = 1;
  new_block->next = block->next;

  block->size = size;
  block->next = new_block;
}

void coalesce() {
  block_t *current = head;

  while (current && current->next) {
    char *current_end = (char *)current + BLOCK_SIZE + current->size;

    if (current->free && current->next->free &&
        current_end == (char *)current->next) {
      current->size += BLOCK_SIZE + current->next->size;

      current->next = current->next->next;
    } else {
      current = current->next;
    }
  }
}

void *zmalloc(size_t size) {
  if (size == 0)
    return NULL;

  size = align_size(size);

  block_t *block;

  if (!head) {
    block = request_space(size);

    if (!block)
      return NULL;

    head = block;
  } else {
    block = find_free_block(size);

    if (block) {
      split_block(block, size);
      block->free = 0;
    } else {
      block_t *last = head;

      while (last->next)
        last = last->next;

      block = request_space(size);

      if (!block)
        return NULL;

      last->next = block;
    }
  }

  return (void *)((char *)block + BLOCK_SIZE);
}

void zfree(void *ptr) {
  if (!ptr)
    return;

  block_t *block = (block_t *)((char *)ptr - BLOCK_SIZE);
  block->free = 1;

  coalesce();
}

void *zcalloc(size_t nmemb, size_t size) {
  if (size != 0 && nmemb > SIZE_MAX / size)
    return NULL;

  size_t total = nmemb * size;

  void *ptr = zmalloc(total);

  if (ptr)
    memset(ptr, 0, total);

  return ptr;
}

void *zrealloc(void *ptr, size_t size) {
  if (!ptr)
    return zmalloc(size);

  if (size == 0) {
    zfree(ptr);
    return NULL;
  }

  size = align_size(size);

  block_t *block = (block_t *)((char *)ptr - BLOCK_SIZE);

  if (block->size >= size) {
    split_block(block, size);
    return ptr;
  }

  if (block->next && block->next->free &&
      ((char *)block + BLOCK_SIZE + block->size == (char *)block->next)) {

    size_t combined = block->size + BLOCK_SIZE + block->next->size;

    if (combined >= size) {
      block->size = combined;
      block->next = block->next->next;

      split_block(block, size);

      return ptr;
    }
  }

  void *new_ptr = zmalloc(size);

  if (!new_ptr)
    return NULL;

  memcpy(new_ptr, ptr, block->size);
  zfree(ptr);

  return new_ptr;
}
