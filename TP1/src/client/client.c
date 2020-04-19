#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"

#define TAM 256

int main(int argc, char *argv[]) 
{
	if (argc < 3) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit( 0 );
	}

	const int sockfd = tcp_connect(argv[1], argv[2]);

	if (sockfd > 0)	{
		authenticate(sockfd);
	}
	else {
		perror("Conexión");
	}

	return EXIT_SUCCESS;
} 
