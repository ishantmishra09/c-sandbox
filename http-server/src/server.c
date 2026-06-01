#include "../include/threadpool.h"

#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#define PORT 3000
#define BACKLOG 10
#define STATIC_DIR "static"

static const char *mime_type(const char *path) {

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
  if (strcmp(ext, ".iso") == 0)
    return "image/x-icon";

  return "text/plain";
}

static char *read_file(const char *path, long *out_size) {

  FILE *fp = fopen(path, "rb");
  if (fp == NULL) {

    perror("read_file: Error opening file");
    return NULL;
  }

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
  if (bytes != (size_t)*out_size) {

    fclose(fp);
    free(buf);
    return NULL;
  }

  fclose(fp);
  return buf;
}

static void handle_client(int client_fd) {

  char request[2048];
  ssize_t n = recv(client_fd, request, sizeof(request) - 1, 0);
  if (n <= 0) {

    close(client_fd);
    return;
  }
  request[n] = '\0';

  char method[8], url_path[256];
  if (sscanf(request, "%7s %255s", method, url_path) != 2) {

    close(client_fd);
    return;
  }

  char fs_path[512];
  if (strcmp(url_path, "/") == 0)
    snprintf(fs_path, sizeof(fs_path), "%s/index.html", STATIC_DIR);
  else
    snprintf(fs_path, sizeof(fs_path), "%s%s", STATIC_DIR, url_path);

  if (strstr(fs_path, "..")) {

    const char *resp = "HTTP/1.1 400 Bad Request\r\n\r\nBad Path";
    send(client_fd, resp, strlen(resp), 1);
    close(client_fd);
    return;
  }

  long file_size = 0;
  char *body = read_file(fs_path, &file_size);
  if (body == NULL) {

    const char *page = "<html><body><h1>404 Not Found</h1></body></html>";
    char header[256];
    int hlen = snprintf(header, sizeof(header),
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: %zu\r\n"
                        "Connection: close\r\n\r\n",
                        strlen(page));

    send(client_fd, header, hlen, 0);
    send(client_fd, page, strlen(page), 0);
    close(client_fd);
    return;
  }

  char header[256];
  int hlen = snprintf(header, sizeof(header),
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: %s\r\n"
                      "Content-Length: %ld\r\n"
                      "Connection: close\r\n\r\n",
                      mime_type(fs_path), file_size);

  send(client_fd, header, hlen, 0);
  send(client_fd, body, file_size, 0);
  free(body);
  close(client_fd);
}

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {

    perror("socket\n");
    return 1;
  }

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_fd, (struct sockaddr *)&server_addr,
           (socklen_t)sizeof(server_addr)) < 0) {

    perror("bind");
    close(server_fd);
    return 1;
  }

  if (listen(server_fd, BACKLOG) < 0) {

    perror("listen");
    close(server_fd);
    return 1;
  }

  printf("Server is running on :%d\n", PORT);

  ThreadPool *tpool = tpool_create();

  while (1) {

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {

      perror("accept");
      continue;
    }

    tpool_add_task(tpool, handle_client, client_fd);
  }

  close(server_fd);
  return 0;
}
