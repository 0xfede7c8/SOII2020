#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <netdb.h> 
#include <stdint.h>

/**
 * Conectarse mediante TCP/IP a un servidor
 *
 * @param ip un c-string conteniendo la dirección IP del servidor
 * @param port un c-string conteniendo el puerto a conectar
 * @return Devuelve el mismo tipo de valor que <sys/socket.h> connect()
 */
int tcp_connect(const char* ip, const char* port);

/**
 * Conectarse mediante TCP/IP a un servidor
 *
 * @param ip un struct hostent* conteniendo la IP del servidor
 * @param port uint16_t conteniendo el puerto a conectar
 * @return Devuelve el mismo tipo de valor que <sys/socket.h> connect()
 */
int tcp_connect_raw(const struct hostent* ip, const uint16_t port);


/**
 * Crea un servidor TCP/IP
 *
 * @param @param port un c-string conteniendo el puerto a bindear
 * @return Devuelve el mismo tipo de valor que <sys/socket.h> listen()
 */
int tcp_server_raw(const char* port);

/**
 * Crea un servidor TCP/IP
 *
 * @param port uint16_t conteniendo el puerto a conectar
 * @return Devuelve el mismo tipo de valor que <sys/socket.h> listen()
 */
int tcp_server(const uint16_t port);

#endif