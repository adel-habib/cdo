#include "../headers/socket.h"
#include "../headers/http.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
int main() {

  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

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
      retval =
          accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
      if (retval == -1) {
        perror("accept()");
        continue;
      } else {
        if (retval > max_fd) {
          max_fd = retval;
        }
        FD_SET(retval, &sockets);
        char client_addr_str[200];

        sockaddr_to_string(&client_addr, client_addr_str,
                           sizeof(client_addr_str));
        printf("Accepted new connection. Client address: %s. Cleint fd: %d \n",
               client_addr_str, retval);
      }
    }

    // handle client request
    for (int i = server_fd + 1; i < max_fd; i++) {
      if (FD_ISSET(i, &sockets)) {
        char buffer[BUFFER_SIZE];
        retval = recv(i, &buffer, BUFFER_SIZE, 0);
        if (retval <= 0) {
          perror("recv\n");
          close(i);
          FD_CLR(i, &sockets);
          printf("closed connection and cleared fd %d\n", i);
        }
        printf("read %d bytes from client with fd: %d. message: %s\n", retval,
               i, buffer);

          send(i, http_404_content, strlen(http_404_content), 0);
        close(i);
        FD_CLR(i, &sockets);
      }
    }
  }
  close(server_fd);
}
