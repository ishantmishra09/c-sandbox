#ifndef DYNARR_H
#define DYNARR_H

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  int *data;
  size_t size;
  size_t capacity;
} dynarr_t;

bool dynarr_init(dynarr_t *arr);
void dynarr_free(dynarr_t *arr);

bool dynarr_push(dynarr_t *arr, int value);
bool dynarr_pop(dynarr_t *arr);

bool dynarr_get(const dynarr_t *arr, size_t index, int *out);
bool dynarr_set(dynarr_t *arr, size_t index, int value);

void dynarr_fmt(const dynarr_t *arr);

#endif
