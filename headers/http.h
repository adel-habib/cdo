#ifndef HTTP_H
#define HTTP_H

const char *http_404_content = \
                                "HTTP/1.0 404 Not Found\r\n"
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
#endif
