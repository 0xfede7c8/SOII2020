#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "tcp_connection.h"

int TCPConnectRaw(const struct hostent* server, const uint16_t port)
{
	int result = -1;
	struct sockaddr_in serv_addr;

	const int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd != -1) {
		memset((char *) &serv_addr, '0', sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, (size_t)server->h_length);
		serv_addr.sin_port = htons((uint16_t)port);

		result = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));	
	}
	
	return (result < 0) ? result : sockfd; 
}

int TCPConnect(const char* ip, const char* port)
{
	const uint16_t port_int = (uint16_t)atoi(port);
	const struct hostent *server = gethostbyname(ip);
	
	// To have the same return value as connect
	return (server == NULL) ? -1 : TCPConnectRaw(server, port_int);
;}

int TCPServer(const uint16_t port)
{
	int result = -1;
	struct sockaddr_in serv_addr;

	const int sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd != -1) { 
		memset((char *) &serv_addr, 0, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		result = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
		if (result == 0) {
			result = listen(sockfd, 5);
		}
	}
	return (result < 0) ? result : sockfd;
}

int TCPServerRaw(const char* port)
{
	const uint16_t port_int = (uint16_t)atoi(port);
	return TCPServer(port_int);
}

int TCPAccept(const int sockfd)
{
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	return accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
}

int createServerAndAccept(const char* port, int *listenFd)
{	
	int result = -1;
	*listenFd = TCPServerRaw(port); 
	if (*listenFd != -1)
	{
		result = TCPAccept(*listenFd);
	}
	return result;
}
