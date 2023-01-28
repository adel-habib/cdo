#include "../headers/http.h"
#include "../headers/socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]) {

    uint16_t port = 8080;
    char* interface = "127.0.0.1";
    int iteration = 0;

    char *endptr;
    if (argc > 1) {
        errno = 0;
        long int temp_port = strtol(argv[1], &endptr, 10);
        if (errno != 0 || *endptr != '\0' || temp_port < 0 || temp_port > UINT16_MAX) {
            printf("Error: Invalid port number provided\n");
            return 1;
        }
        port = (uint16_t) temp_port;
    }

  int server_fd = listen_TCP(interface, port);

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
        printf("socket %d is ready for read\n", i);
        http_request_t *req = parse_http(i);
        if (req == NULL) {
          printf("INVALID HTTP REQUEST!\n");
          close(i);
          FD_CLR(i, &sockets);
        } else {
            iteration++;
          char *request = request_to_string(req);
          // printf("got the following request: \n%s\n\n", request);
          // printf("will respond with %s \n", http_404_not_found);
          free(req);
          send(i, http_welcome, strlen(http_welcome), 0);
          printf("sent response!, iteration number: %d\n",iteration);
          close(i);
          FD_CLR(i, &sockets);
        }
      }
    }
  }
  close(server_fd);
}
