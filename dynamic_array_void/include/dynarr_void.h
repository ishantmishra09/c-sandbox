#ifndef DYNARR_VOID_H
#define DYNARR_VOID_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  void *data;
  size_t size;
  size_t capacity;
  size_t elem_size;
} dynarr_t;

bool dynarr_init(dynarr_t *arr, size_t elem_size);
void dynarr_free(dynarr_t *arr);

bool dynarr_push(dynarr_t *arr, const void *value);
bool dynarr_pop(dynarr_t *arr, void *out);

bool dynarr_get(const dynarr_t *arr, size_t index, void *out);
bool dynarr_set(dynarr_t *arr, size_t index, const void *value);

void dynarr_fmt(const dynarr_t *arr, void (*fmt_elem)(const void *));

#endif
