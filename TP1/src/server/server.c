#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"

void printHelp(const int argc, char *argv[])
{
	if (argc < 2) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit(1);
	}
}

int main(int argc, char *argv[])
{
	
	printHelp(argc, argv);

	const int sockfd = createServerAndAccept(argv[1]);

	if (sockfd > 0) {
		if (authenticate(sockfd)) {

		}
		
	} else {
		perror("[-] Problema creando servidor. Saliendo");
		exit(1);
	}

	close(sockfd);
	return EXIT_SUCCESS; 
} 
