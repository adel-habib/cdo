#ifndef REST_H
#define REST_H

#include "http.h"
typedef struct {
  char method[8];
  char uri[128];
  void (*handler)(http_request_t *req);
} endpoint;


#endif
