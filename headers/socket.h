#ifndef SOCKET_H
#define SOCKET_H
#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

/*
* listen_TCP - creates a TCP socket, binds it to a specified address and port, and listens for incoming connections
*
* @address: a pointer to a null-terminated string representing the IP address in the dot-decimal notation 
* (e.g. "127.0.0.1" for localhost)
* @port: the port number in host byte order
* Returns: a file descriptor representing the listening socket on success
*/
int listen_TCP(const char* address, uint16_t port);


/*
* create_sockaddr - creates a socket address structure for an IPv4 address and port pair 
* @address: a pointer to a null-terminated string representing the IP address in the dot-decimal notation 
* (e.g. "127.0.0.1" for localhost)
* @port: the port number in host byte order
* Returns: a sockaddr_in struct filled with the specified address and port in network byte order
*/
struct sockaddr_in create_sockaddr(const char* address, uint16_t port);

/*
 * sockaddr_to_string - converts a sockaddr_in struct to a string in the format "ip:port"
 * and writes the string to the provided buffer
 *
 * @addr: a pointer to a sockaddr_in struct
 * @buffer: a buffer to store the resulting string
 * @buffer_size: the size of the buffer
 *
*/
void sockaddr_to_string(const struct sockaddr_in* addr, char* buffer, size_t buffer_size);

#endif
