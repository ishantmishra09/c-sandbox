#include "../include/dynarr_void.h"

#include <stdio.h>
#include <stdlib.h>

static void fmt_float(const void *e) { printf("%.2f", *(const float *)e); }

int main(void) {

  dynarr_t floats;

  if (!dynarr_init(&floats, sizeof(float))) {

    fputs("Failed to initialise array\n", stderr);
    return EXIT_FAILURE;
  }

  for (float i = 1; i < 8; i++) {

    if (!dynarr_push(&floats, &i)) {

      fputs("Push failed\n", stderr);
      dynarr_free(&floats);
      return EXIT_FAILURE;
    }
  }
  dynarr_fmt(&floats, fmt_float);

  if (!dynarr_pop(&floats, NULL)) {
    fputs("Pop failed\n", stderr);
    dynarr_free(&floats);
    return EXIT_FAILURE;
  }
  dynarr_fmt(&floats, fmt_float);

  float out;
  if (!dynarr_get(&floats, 0, &out)) {

    fputs("Get failed\n", stderr);
    dynarr_free(&floats);
    return EXIT_FAILURE;
  }
  printf("First element: %.2f\n", out);

  float replacement = 999.99;
  if (!dynarr_set(&floats, 0, &replacement)) {

    fputs("Set failed\n", stderr);
    dynarr_free(&floats);
    return EXIT_FAILURE;
  }
  dynarr_fmt(&floats, fmt_float);

  dynarr_free(&floats);
  return EXIT_SUCCESS;
}
