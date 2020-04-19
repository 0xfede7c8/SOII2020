#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"

#define TAM 256

int main(int argc, char *argv[]) 
{
	if (argc < 3) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit( 0 );
	}

	const int sockfd = tcp_connect(argv[1], argv[2]);

	int terminar = 0;	
	char buffer[TAM];
	ssize_t n;

	while(1)
	{
		printf( "Ingrese el mensaje a transmitir: " );
		memset( buffer, '\0', TAM );
		fgets( buffer, TAM-1, stdin );

		n = write( sockfd, buffer, strlen(buffer) );
		if (n < 0)
		{
			perror( "escritura de socket" );
			exit( 1 );
		}

		// Verificando si se escribió: fin
		buffer[strlen(buffer)-1] = '\0';
		if( !strcmp( "fin", buffer ) )
		{
			terminar = 1;
		}

		memset( buffer, '\0', TAM );
		n = read( sockfd, buffer, TAM );
		if (n < 0)
		{
			perror( "lectura de socket" );
			exit( 1 );
		}
		printf( "Respuesta: %s\n", buffer );
		if(terminar)
		{
			printf( "Finalizando ejecución\n" );
			exit(0);
		}
	}
	return 0;
} 
