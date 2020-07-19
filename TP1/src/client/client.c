#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "tcp_connection.h"
#include "authenticate.h"
#include "prompt.h"
#include "command_line.h"
#include "utils.h"

int sockfd;    // Necesita ser global para que el signal handler la pueda ver

void sigintHandler(int sig_num) 
{ 	
	(void)sig_num;    // Descartamos el uso de sig_num para remover los warnings
    fflush(stdout);
    safeExit(sockfd); 
} 


void printHelp(const int argc, char *argv[])
{
	if (argc < 3) {
		fprintf( stderr, "Uso %s host puerto\n", argv[0]);
		exit(0);
	}
}

int main(int argc, char *argv[]) 
{
	printHelp(argc, argv);

	signal(SIGINT, sigintHandler);    // Handleamos la señal SIGINT con una callback definida por nosotros

	sockfd = TCPConnect(argv[1], argv[2]);

	if (sockfd > 0)	{
		if (authenticate(sockfd, sockfd)) {
			setPromptUsername(getAutheticatedUsername());
			printASCIIArt();
			while (1) {
				printPrompt();
				char* command = getCommand();
				if (command != NULL) {
					char* argv[MAX_CMD_ARGS];
					int argc;
					splitCommand(command, argv, &argc);
					if (!runCommand(argc, argv, sockfd)) {
						printf("[-] Comando no reconocido: %s\n", command);
					}
				}
			}
		} else {
			printf("[-] Error de autenticación. Saliendo.");
		}
	}
	else {
		perror("[-] Problema en la conexión. Saliendo");
	}

	close(sockfd);
	return EXIT_SUCCESS;
} 
