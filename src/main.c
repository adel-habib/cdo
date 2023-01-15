#include "../headers/http.h"
#include "../headers/socket.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#define BUFFER_SIZE 1024

int main() {


  int server_fd = listen_TCP("127.0.0.1", 8090);

  fd_set sockets, ready_sockets;
  FD_ZERO(&sockets);
  FD_SET(server_fd, &sockets);
  int max_fd = server_fd + 1;
  while (1) {
    ready_sockets = sockets;
    int retval = select(max_fd, &ready_sockets, NULL, NULL, NULL);
    if (retval == -1) {
      perror("select()");
      exit(EXIT_FAILURE);
    }
    // handle new connection
    if (FD_ISSET(server_fd, &ready_sockets)) {
      handle_new_connection(server_fd, &max_fd, &sockets);
    }

    // handle client request
    for (int i = server_fd + 1; i < max_fd; i++) {
      if (FD_ISSET(i, &sockets)) {
        printf("socket %d is ready for read", i);
        http_request_t *req = parse_http(i);
        if (req == NULL) {
          printf("INVALID HTTP REQUEST!\n");
          close(i);
          FD_CLR(i, &sockets);
        } else {
          char *request = request_to_string(req);
          printf("got the following request: \n%s\n\n", request);
          printf("will respond with %s \n", http_404_not_found);
          free(req);
          send(i, http_404_not_found, strlen(http_404_not_found), 0);
          printf("sent response!\n");
          close(i);
          FD_CLR(i, &sockets);
        }
      }
    }
  }
  close(server_fd);
}
