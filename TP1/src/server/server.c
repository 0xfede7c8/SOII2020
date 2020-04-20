#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"

void print_help(const int argc, char *argv[])
{
	if (argc < 2) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit(1);
	}
}

int main(int argc, char *argv[]) {
	
	print_help(argc, argv);

	const int newsockfd = create_server_and_accept(argv[1]);

	if (newsockfd < 0) {
		perror( "creacion de server" );
		exit(1);
	}

	authenticate(newsockfd);

	return EXIT_SUCCESS; 
} 
