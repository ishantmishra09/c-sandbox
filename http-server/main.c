#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_LENGTH 1024
#define PORT 3000
#define BACKLOG 10

int main() {

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {

    perror("Socket creation failed\n");
    return EXIT_FAILURE;
  }

  struct sockaddr_in server_addr;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  int opt = 1;
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  if (bind(server_fd, (struct sockaddr *)&server_addr,
           (socklen_t)sizeof(server_addr)) < 0) {

    perror("Socket bind failed\n");
    close(server_fd);
    return EXIT_FAILURE;
  }

  if (listen(server_fd, BACKLOG) < 0) {

    perror("Listening failed\n");
    close(server_fd);
    return EXIT_FAILURE;
  }

  fprintf(stdout, "Server listening on :%d", PORT);
  while (1) {

    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    int client_fd =
        accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd < 0) {

      perror("Accept failed\n");
      continue;
    }

    char request[MAX_BUFFER_LENGTH];

    ssize_t bytes_read = recv(client_fd, request, sizeof(request) - 1, 0);
    if (bytes_read > 0) {

      request[bytes_read] = '\0';

      printf("HTTP Request\n%s\n", request);

      const char *body = "<html>"
                         "<body>"
                         "<h1>Hello World</h1>"
                         "</body>"
                         "</html>";

      char response[2048];

      int response_len = snprintf(response, sizeof(response),
                                  "HTTP/1.1 200 OK\r\n"
                                  "Content-Type: text/html\r\n"
                                  "Content-Length: %zu\r\n"
                                  "Connection: close\r\n"
                                  "\r\n"
                                  "%s",
                                  strlen(body), body);

      send(client_fd, response, response_len, 0);
    }

    close(client_fd);
  }

  close(server_fd);
  return EXIT_SUCCESS;
}
