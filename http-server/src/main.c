#include "../include/handler.h"
#include "../include/threadpool.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 3000
#define BACKLOG 10

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

  printf("[server] running on :%d\n", PORT);

  ThreadPool *tpool = tpool_create();
  if (tpool == NULL) {

    close(server_fd);
    return 1;
  }

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

  tpool_destroy(tpool);
  close(server_fd);
  return 0;
}
