#include "../include/mime.h"

#include <string.h>

const char *mime_type(const char *path) {

  const char *ext = strrchr(path, '.');
  if (ext == NULL)
    return "application/octet-stream";

  if (strcmp(ext, ".html") == 0)
    return "text/html";
  if (strcmp(ext, ".css") == 0)
    return "text/css";
  if (strcmp(ext, ".js") == 0)
    return "application/javascript";
  if (strcmp(ext, ".png") == 0)
    return "image/png";
  if (strcmp(ext, ".jpg") == 0)
    return "image/jpeg";
  if (strcmp(ext, ".jpeg") == 0)
    return "image/jpeg";
  if (strcmp(ext, ".ico") == 0)
    return "image/x-icon";
  if (strcmp(ext, ".json") == 0)
    return "application/json";
  if (strcmp(ext, ".svg") == 0)
    return "image/svg+xml";

  return "text/plain";
}
