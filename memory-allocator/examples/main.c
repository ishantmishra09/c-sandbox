#include "../include/allocator.h"

#include <stdio.h>

int main(void) {
  int *arr = zmalloc(11 * sizeof(int));

  for (int i = 0; i < 11; i++)
    arr[i] = i;

  arr = zrealloc(arr, 21 * sizeof(int));

  for (int i = 11; i < 21; i++)
    arr[i] = i;

  for (int i = 0; i < 21; i++)
    printf("%d ", arr[i]);
  printf("\n");

  zfree(arr);

  return 0;
}
