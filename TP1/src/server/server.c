#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"

/**
 * Crea un servidor TCP/IP y acepta una conexión entrante
 *
 * @param port puntero al c-string con el puerto
 * @return El file descriptor de la nueva conexión o -1 en caso de error. Se setea errno.
 */
int create_server_and_accept(const char* port)
{
	int result = tcp_server_raw(port); 
	if (result != -1)
	{
		result = tcp_accept(result);
	}
	return result;
}

int main( int argc, char *argv[] ) {
	
	if (argc < 2) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit(1);
	}

	const int newsockfd = create_server_and_accept(argv[1]);

	if (newsockfd < 0) {
		perror( "creacion de server" );
		exit(1);
	}

	authenticate(newsockfd);

	return EXIT_SUCCESS; 
} 
