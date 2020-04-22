#ifndef UTILS_H

#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "message_transmission.h"

void safeExit(const int fd) 
{
	printf("\nEnviando CLIENT_EXIT al servidor...\n"); 
	sendMessage(fd, CLIENT_EXIT);
	close(fd);
	printf("Saliendo...\n");
	exit(0);
}

#endif