#include "../headers/http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

bool is_valid_http_method(char *method) {
  const char *valid_methods[] = {"GET",     "HEAD",   "POST",
                                 "PUT",     "DELETE", "CONNECT",
                                 "OPTIONS", "TRACE",  "PATCH"};

  int num_valid_methods = sizeof(valid_methods) / sizeof(valid_methods[0]);

  // Check if the method is in the list of valid methods
  for (int i = 0; i < num_valid_methods; i++) {
    if (strcmp(method, valid_methods[i]) == 0) {
      return true;
    }
  }

  // If we reach here, the method is not in the list of valid methods
  return false;
}

http_request_t *parse_http(int client_fd) {
  char buffer[1024];
  int nbytes = recv(client_fd, buffer, sizeof(buffer), 0);
  if (nbytes <= 0) {
    perror("recv\n");
    return NULL;
  }
  http_request_t *req = calloc(1, sizeof(http_request_t));

  char *line = strtok(buffer, "\r\n");
  sscanf(line, "%s %s %s", req->method, req->uri, req->version);

  if (!is_valid_http_method(req->method) ||
      (strcmp(req->version, "HTTP/1.1") != 0 &&
       strcmp(req->version, "HTTP/1.0") != 0 &&
       strcmp(req->version, "HTTP/2.0") != 0)) {
    perror("invalid http buffer");
    return NULL;
  }

  snprintf(req->headers, sizeof(req->headers), "%s",
           line + strlen(req->method) + strlen(req->uri) +
               strlen(req->version) + 3);

  // Check for message payload
  int content_length = 0;
  char *content_length_str = strstr(req->headers, "Content-Length: ");
  if (content_length_str != NULL) {
    sscanf(content_length_str, "Content-Length: %d", &content_length);
  }

  // Receive message payload
  if (content_length > 0) {
    char *body_ptr = buffer + nbytes - content_length;
    strncpy(req->payload, body_ptr, sizeof(req->payload));
  }
  req->client_fd = client_fd;
  return req;
}

char *request_to_string(http_request_t *req) {
  char *request_str = NULL;
  int size = snprintf(NULL, 0, "%s %s %s\r\n%s\r\n%s", req->method, req->uri,
                      req->version, req->headers, req->payload);
  request_str = (char *)malloc(size + 1);
  snprintf(request_str, size + 1, "%s %s %s\r\n%s\r\n%s", req->method, req->uri,
           req->version, req->headers, req->payload);
  return request_str;
}

const char *http_404_not_found = "HTTP/1.1 404 Not Found\r\n"
                                 "Content-type: text/html\r\n"
                                 "Connection: close\r\n"
                                 "\r\n"
                                 "<html>"
                                 "<head>"
                                 "<title>CDO: Not Found</title>"
                                 "</head>"
                                 "<body>"
                                 "<h1>Not Found (404)</h1>"
                                 "</body>"
                                 "</html>";

const char *http_close_connection = "HTTP/1.1 200 OK\r\n"
                                    "Connection: Close\r\n"
                                    "\r\n";
