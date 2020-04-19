#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"

#define TAM 256

int main( int argc, char *argv[] ) {
	socklen_t clilen;
	char buffer[TAM];
	struct sockaddr_in cli_addr;
	ssize_t n;

	if (argc < 2) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	const int sockfd = tcp_server_raw(argv[1]);

	if (sockfd < 0)
	{
		perror("Error creando servidor");
		exit(1);
	}

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0) {
		printf("%i", sockfd);
		perror( "accept" );
		exit( 1 );
	}

	memset( buffer, 0, TAM );

	n = read( newsockfd, buffer, TAM-1 );
	if ( n < 0 ) {
		perror( "lectura de socket" );
		exit(1);
	}

	printf( "PROCESO %d. ", getpid() );
	printf( "Recibí: %s", buffer );

	return EXIT_SUCCESS; 
} 
