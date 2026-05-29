#include "../include/dynarr.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {

  dynarr_t ints;

  if (!dynarr_init(&ints)) {

    fputs("Failed to initialise array\n", stderr);
    return EXIT_FAILURE;
  }

  for (int i = 1; i <= 8; i++) {

    if (!dynarr_push(&ints, i)) {

      fputs("Push failed\n", stderr);
      dynarr_free(&ints);
      return EXIT_FAILURE;
    }
  }
  dynarr_fmt(&ints);

  dynarr_pop(&ints);
  dynarr_fmt(&ints);

  int val;
  if (!dynarr_get(&ints, 5, &val)) {

    fputs("Get failed\n", stderr);
    dynarr_free(&ints);
    return EXIT_FAILURE;
  }
  printf("Element at index 5: %d\n", val);

  if (!dynarr_set(&ints, 5, 999)) {

    fputs("Set failed\n", stderr);
    dynarr_free(&ints);
    return EXIT_FAILURE;
  }
  dynarr_fmt(&ints);

  dynarr_free(&ints);
  return EXIT_SUCCESS;
}
