#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "tcp_client.h"

#define BUFFER_SIZE 256u

int tcp_connect_raw(const struct hostent* server, const uint16_t port)
{
	int result;
	struct sockaddr_in serv_addr;

	const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
		result = sockfd;
	} 
	else
	{
		memset((char *) &serv_addr, '0', sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, (size_t)server->h_length);
		serv_addr.sin_port = htons((uint16_t)port);

		result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	}
	
	return result; 
}

int tcp_connect(const char* ip, const char* port)
{
	const uint16_t port_int = (uint16_t)atoi(port);
	const struct hostent *server = gethostbyname(ip);
	
	// To have the same return value as connect
	return (server == NULL) ? -1 : tcp_connect_raw(server, port_int);
;}
