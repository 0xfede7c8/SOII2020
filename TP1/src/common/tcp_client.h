#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <netdb.h> 
#include <stdint.h>

// Returns socket fd value
int tcp_connect(const char* ip, const char* port);
int tcp_connect_raw(const struct hostent* ip, const uint16_t port);

#endif