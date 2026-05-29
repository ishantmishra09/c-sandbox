#include "../include/dynarr.h"

#include <stdio.h>
#include <stdlib.h>

#define DYNARR_MIN_CAPACITY ((size_t)4)

/**
 * Internal Helpers
 */
static bool dynarr_resize(dynarr_t *arr, size_t new_capacity) {

  if (new_capacity < DYNARR_MIN_CAPACITY)
    new_capacity = DYNARR_MIN_CAPACITY;

  int *temp = realloc(arr->data, new_capacity * sizeof(int));
  if (temp == NULL) {

    fprintf(stderr, "dynarr: realloc(%zu elements) failed\n", new_capacity);
    return false;
  }

  arr->data = temp;
  arr->capacity = new_capacity;
  return true;
}

/**
 * Public API
 */
bool dynarr_init(dynarr_t *arr) {

  arr->size = 0;
  arr->capacity = 4;

  arr->data = malloc(arr->capacity * sizeof(int));

  if (arr->data == NULL) {

    fprintf(stderr, "dynarr: initial malloc(%zu elements) failed\n",
            arr->capacity);
    arr->capacity = 0;
    return false;
  }

  return true;
}

void dynarr_free(dynarr_t *arr) {

  free(arr->data);

  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
}

bool dynarr_push(dynarr_t *arr, int value) {

  if (arr->size >= arr->capacity) {

    if (!dynarr_resize(arr, arr->capacity * 2))
      return false;
  }

  arr->data[arr->size++] = value;
  return true;
}

bool dynarr_pop(dynarr_t *arr) {

  if (arr->size == 0)
    return true;

  arr->size--;

  if (arr->capacity > DYNARR_MIN_CAPACITY && arr->size <= arr->capacity / 4) {

    if (!dynarr_resize(arr, arr->capacity / 2)) {

      fprintf(stderr, "dynarr: shrink failed (non-fatal, pop succeeded)\n");
      return false;
    }
  }

  return true;
}

bool dynarr_get(const dynarr_t *arr, size_t index, int *out) {

  if (index >= arr->size) {

    fprintf(stderr, "dynarr_get: index %zu out of bounds (size=%zu)\n", index,
            arr->size);
    return false;
  }

  *out = arr->data[index];
  return true;
}

bool dynarr_set(dynarr_t *arr, size_t index, int value) {

  if (index >= arr->size) {

    fprintf(stderr, "dynarr_get: index %zu out of bounds (size=%zu)\n", index,
            arr->size);
    return false;
  }

  arr->data[index] = value;
  return true;
}

void dynarr_fmt(const dynarr_t *arr) {

  putchar('[');

  for (size_t i = 0; i < arr->size; i++) {

    printf("%d", arr->data[i]);

    if (i + 1 < arr->size)
      fputs(", ", stdout);
  }

  puts("]");
}
