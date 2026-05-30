#include "../include/dynarr_void.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DYNARR_MIN_CAPACITY ((size_t)4)
#define DYNARR_GROWTH_FACTOR ((size_t)2)

/**
 * Internal Helpers
 */
static inline void *elem_ptr(const dynarr_t *arr, size_t i) {

  return (unsigned char *)arr->data + i * arr->elem_size;
}

static bool dynarr_resize(dynarr_t *arr, size_t new_capacity) {

  if (new_capacity < DYNARR_MIN_CAPACITY)
    new_capacity = DYNARR_MIN_CAPACITY;

  // Guard against overflow in the byte-count multiplication
  if (arr->elem_size > 0 && new_capacity > (size_t)-1 / arr->elem_size) {

    fprintf(stderr, "dynarr: resize to %zu elements would overflow\n",
            new_capacity);
    return false;
  }

  void *temp = realloc(arr->data, new_capacity * arr->elem_size);
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
bool dynarr_init(dynarr_t *arr, size_t elem_size) {

  if (arr == NULL || elem_size == 0) {

    fprintf(stderr, "dynarr_init: invalid argument (arr=%p, element_size=%zu)",
            (void *)arr, elem_size);
    return false;
  }

  arr->elem_size = elem_size;
  arr->size = 0;
  arr->capacity = DYNARR_MIN_CAPACITY;

  arr->data = malloc(arr->capacity * arr->elem_size);
  if (arr->data == NULL) {

    fprintf(stderr, "dynarr: initial malloc(%zu elements) failed\n",
            arr->capacity);
    arr->capacity = 0;
    return false;
  }

  return true;
}

void dynarr_free(dynarr_t *arr) {

  if (arr == NULL)
    return;

  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
  arr->elem_size = 0;
}

bool dynarr_push(dynarr_t *arr, const void *value) {

  if (arr == NULL || value == NULL) {

    fprintf(stderr, "dynarr_push: NULL arguments\n");
    return false;
  }

  if (arr->size >= arr->capacity) {

    if (!dynarr_resize(arr, arr->size * DYNARR_GROWTH_FACTOR))
      return false;
  }

  memcpy(elem_ptr(arr, arr->size), value, arr->elem_size);
  arr->size++;
  return true;
}

bool dynarr_pop(dynarr_t *arr, void *out) {

  if (arr == NULL) {

    fprintf(stderr, "dynarr_pop: NULL arguments\n");
    return false;
  }

  if (arr->size == 0) {

    fprintf(stderr, "dynarr_pop: array is empty\n");
    return false;
  }

  if (out != NULL)
    memcpy(out, elem_ptr(arr, arr->size - 1), arr->elem_size);

  arr->size--;

  if (arr->capacity > DYNARR_MIN_CAPACITY && arr->size <= arr->capacity / 4) {

    if (!dynarr_resize(arr, arr->capacity / DYNARR_GROWTH_FACTOR)) {

      fprintf(stderr, "dynarr: shrink failed (non-fatal, pop succeeded)\n");
      return false;
    }
  }
  return true;
}

bool dynarr_get(const dynarr_t *arr, size_t index, void *out) {

  if (arr == NULL || out == NULL) {

    fprintf(stderr, "dynarr_get: NULL arguments\n");
    return false;
  }

  if (index >= arr->size) {

    fprintf(stderr, "dynarr_get: index %zu out of bounds (size=%zu)\n", index,
            arr->size);
    return false;
  }

  memcpy(out, elem_ptr(arr, index), arr->elem_size);
  return true;
}

bool dynarr_set(dynarr_t *arr, size_t index, const void *value) {

  if (arr == NULL || value == NULL) {

    fprintf(stderr, "dynarr_set: NULL arguments\n");
    return false;
  }

  if (index >= arr->size) {

    fprintf(stderr, "dynarr_set: index %zu out of bounds (size=%zu)\n", index,
            arr->size);
    return false;
  }

  memcpy(elem_ptr(arr, index), value, arr->elem_size);
  return true;
}

void dynarr_fmt(const dynarr_t *arr, void (*fmt_elem)(const void *)) {

  if (arr == NULL || fmt_elem == NULL) {

    fprintf(stderr, "dynarr_fmt: NULL argument\n");
    return;
  }

  putchar('[');

  for (size_t i = 0; i < arr->size; i++) {

    fmt_elem(elem_ptr(arr, i));
    if (i + 1 < arr->size)
      fputs(", ", stdout);
  }

  puts("]");
}
