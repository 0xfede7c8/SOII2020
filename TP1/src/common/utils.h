/**
 * @file
 * @brief Utilidades generales
 */
#ifndef UTILS_H

#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

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

/**
 * Retorna un nuevo string con el localtime 
 * @param fd descriptor de comunicación con el servidor
 */
char* getCurrentTime()
{
  time_t rawtime;
  time(&rawtime);
  return asctime(localtime(&rawtime));
}

#endif