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

int tcp_server(const uint16_t port)
{
	int sockfd, newsockfd, pid;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	ssize_t n;

	const int sockfd = socket( AF_INET, SOCK_STREAM, 0);
	int result = (sockfd == 0);

	if (result) { 
		
	}
	return result;

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = (uint16_t)atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}

        printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 5 );
}

int tcp_server_raw(const char* port)
{
	const uint16_t port_int = (uint16_t)atoi(port);
	// To have the same return value as connect
	return (server == NULL) ? -1 : tcp_server(port_int);
}
