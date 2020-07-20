#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "message_transmission.h"

#define MAX_MESSAGE_LENGTH 1024

void checkForExit(Message message)
{
	if (message == CLIENT_EXIT)
	{
		printf("Recibimos CLIENT_EXIT. Saliendo...\n");
		exit(0);
	}
}

Message sendMessage(const int fd, const Message message)
{
	const ssize_t n = write(fd, &message, sizeof(Message));
	return checkMessageSend(n);
}

Message receiveMessage(const int fd)
{	
	Message message = MESSAGE_FAILED;
	ssize_t n = 0u;

	while (n == 0) {
		/* Según el largo decidimos si el mensaje recibido es un comando 
		   y lo almacenamos o no */
		const size_t msgSize = sizeof(Message);
		n = read(fd, (void *)&message, msgSize);
		if (n == msgSize) {
			/* Checkeamos si debemos salir del programa */
			checkForExit(message);
		} else {
			/* No deberiamos recibir más que el tamaño de Message.
		   	   Si esto ocurre es que estamos tratando de recibir un comando
		   	   cuando el mensaje en realidad es más largo. En definitiva un bug
		       en la implementación del protocolo */
			message = MESSAGE_FAILED;
		}
	}

	return message;
}

Message sendCredentials(const int fd, const Credentials* credentials)
{
	const ssize_t n = write(fd, (void *)credentials, sizeof(Credentials));
	return checkMessageSend(n);
}

Message receiveCredentials(const int fd, Credentials* credentials)
{
	const size_t credsz = sizeof(Credentials);
	const ssize_t n = read(fd, (void *)credentials, credsz);
	return (n == (ssize_t)credsz) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

int messageOk(Message message)
{
	return message == MESSAGE_SUCCESS;
}

Message sendUserList(const int fd, char* strings[], const size_t len)
{
	Message status = MESSAGE_SUCCESS;
	uint32_t i = 0;
	while( (i<len) && (status == MESSAGE_SUCCESS))
	{
		/* Avisamos que tenemos un usuario para enviar */
		status = sendMessage(fd, USER_LIST);
		if (status) {
			/* Enviamos el usuario */
			const ssize_t n = write(fd, strings[i], MAX_STRINGS_SIZE);
			status = checkMessageSend(n);
		}
		i++;
	}
	/* Cuando terminamos de mandar avisamos que ya no hay mas strings */
	if (status == MESSAGE_SUCCESS)
	{
		status = sendMessage(fd, USER_LIST_FINISH);
	} else {
		perror("[-] Error mandando lista de usuarios");
	}
	return status;
}

Message receiveUserList(const int fd, void (*callback)(const char*))
{
	Message status;
	char user[MAX_STRINGS_SIZE];

	bool finish = false;

	while (!finish) {
		status = receiveMessage(fd);
		if (status == USER_LIST) {
			ssize_t n = 0u;
			/* Esperamos hasta que haya mensaje */
			while (n <= 0) {
				n = read(fd, (void *)user, MAX_STRINGS_SIZE);
				if (n > 0) {
					callback(user);
				}
			}
		}
		finish = status == USER_LIST_FINISH || 
				 status == MESSAGE_FAILED;
	}

	return (status == USER_LIST_FINISH) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}