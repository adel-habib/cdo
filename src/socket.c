#include "../headers/socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
int listen_TCP(const char *address, uint16_t port) {
  struct sockaddr_in server_address = create_sockaddr(address, port);
  int server_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (server_file_descriptor <= 0) {
    perror("socket() failed to create a file descriptor\n");
    exit(EXIT_FAILURE);
  }

  int status = bind(server_file_descriptor, (struct sockaddr *)&server_address,
                    sizeof(server_address));
  if (status < 0) {
    char errMsg[200];
    snprintf(errMsg, sizeof(errMsg),
             "bind(): failed to bind file descriptor (tcp socket) %d to "
             "address %s:%u\n",
             server_file_descriptor, address, port);
    perror(errMsg);
    exit(EXIT_FAILURE);
  }

  status = listen(server_file_descriptor, SOMAXCONN);
  if (status < 0) {
    char errMsg[200];
    snprintf(errMsg, sizeof(errMsg),
             "listen(): failed on file descriptor (tcp socket) %d to listen on "
             "address %s:%u\n",
             server_file_descriptor, address, port);
    perror(errMsg);
    exit(EXIT_FAILURE);
  }

  return server_file_descriptor;
}
struct sockaddr_in create_sockaddr(const char *address, uint16_t port) {
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  if (address == NULL) {
    addr.sin_addr.s_addr = htonl(IN_LOOPBACKNET);
  } else {
    if (inet_pton(AF_INET, address, &addr.sin_addr) <= 0) {
      perror("inet_pton falied!\n");
      exit(EXIT_FAILURE);
    }
  }
  return addr;
}

void sockaddr_to_string(const struct sockaddr_in *addr, char *buffer,
                        size_t buffer_size) {
  char ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
  uint16_t port = ntohs(addr->sin_port);
  snprintf(buffer, buffer_size, "%s:%u", ip, port);
}
