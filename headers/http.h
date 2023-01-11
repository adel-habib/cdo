#ifndef HTTP_H
#define HTTP_H
#include <stdbool.h>


typedef struct {
  char method[8];
  char uri[128];
  char version[16];
  char headers[1024];
  char payload[1024];
} http_request_t;

http_request_t* parse_http(int client_fd);
char* request_to_string(http_request_t* req);
bool is_valid_http_method(char* method);

extern const char *http_404_not_found;
extern const char *http_close_connection;
                               
#endif
