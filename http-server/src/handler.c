#include "../include/handler.h"
#include "../include/file_utils.h"
#include "../include/mime.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define STATIC_DIR "static"
#define RECV_BUF 2048

static void send_response(int fd, int code, const char *status,
                          const char *ctype, const char *body, long body_len) {

  char header[256];
  int hlen = snprintf(header, sizeof(header),
                      "HTTP/1.1 %d %s\r\n"
                      "Content-Type: %s\r\n"
                      "Content-Length: %ld\r\n"
                      "Connection: close\r\n\r\n",
                      code, status, ctype, body_len);

  send(fd, header, hlen, 0);
  send(fd, body, body_len, 0);
}

void handle_client(int client_fd) {

  char request[RECV_BUF];
  ssize_t n = recv(client_fd, request, sizeof(request) - 1, 0);
  if (n <= 0) {

    close(client_fd);
    return;
  }
  request[n] = '\0';

  char method[8], url_path[256];
  if (sscanf(request, "%7s %255s", method, url_path) != 2) {

    const char *body = "<h1>400 Bad Request</h1>";
    send_response(client_fd, 400, "Bad Request", "text/html", body,
                  strlen(body));
    close(client_fd);
    return;
  }

  // Block directory traversal
  if (strstr(url_path, "..")) {

    const char *body = "<h1>403 Forbidden</h1>";
    send_response(client_fd, 403, "Forbidden", "text/html", body, strlen(body));
    close(client_fd);
    return;
  }

  char fs_path[512];
  if (strcmp(url_path, "/") == 0)
    snprintf(fs_path, sizeof(fs_path), "%s/index.html", STATIC_DIR);
  else
    snprintf(fs_path, sizeof(fs_path), "%s%s", STATIC_DIR, url_path);

  long file_size = 0;
  char *body = read_file(fs_path, &file_size);
  if (body == NULL) {

    const char *not_found = "<h1>404 Not Found</h1>";
    send_response(client_fd, 404, "Not Found", "text/html", not_found,
                  strlen(not_found));
    close(client_fd);
    return;
  }

  send_response(client_fd, 200, "OK", mime_type(fs_path), body, file_size);
  free(body);
  close(client_fd);
}
