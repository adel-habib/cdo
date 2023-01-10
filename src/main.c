#include "../headers/socket.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {

	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);

	int server_fd = listen_TCP("127.0.0.1", 8090);

	while (1) {
		int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
		if (client_fd <= 0) {
			perror("failed to accept a new connection\n");
			close(client_fd);
			break;

		}
		char client_addr_str[100];
		sockaddr_to_string(&client_addr, client_addr_str, sizeof(client_addr_str));
		printf("accepted a new connection, fd: %d - ip %s: \n",client_fd,client_addr_str);
		close(client_fd);
		break;
	}
	close(server_fd);
} 
