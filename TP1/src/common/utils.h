/**
 * @file
 * @brief Utilidades generales
 */
#ifndef UTILS_H

#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#include "message_transmission.h"

/**
 * Sale del programa, enviando comando de finalización, 
 * cerrando los descriptors abiertos e imprimiendo mensajes necesarios
 * @param fd descriptor de comunicación con el servidor
 */
void safeExit(const int fd) 
{
	printf("\nEnviando CLIENT_EXIT al servidor...\n"); 
	sendMessage(fd, CLIENT_EXIT);
	close(fd);
	printf("Saliendo...\n");
	exit(0);
}

#endif