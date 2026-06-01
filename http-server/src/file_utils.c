#include "../include/file_utils.h"

#include <stdio.h>
#include <stdlib.h>

char *read_file(const char *path, long *out_size) {

  FILE *fp = fopen(path, "rb");
  if (fp == NULL)
    return NULL;

  fseek(fp, 0, SEEK_END);
  *out_size = ftell(fp);
  rewind(fp);

  char *buf = (char *)malloc(*out_size);
  if (buf == NULL) {

    perror("read_file: malloc\n");
    fclose(fp);
    return NULL;
  }

  size_t bytes = fread(buf, 1, *out_size, fp);
  fclose(fp);

  if (bytes != (size_t)*out_size) {

    fprintf(stderr, "read_file: short read on '%s'\n", path);
    free(buf);
    return NULL;
  }

  return buf;
}
