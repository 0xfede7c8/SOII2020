#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "message_transmission.h"

/**
 * Helper para checkear el status de write
 */
Message checkMessageSend(const ssize_t n)
{
	return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

void checkForExit(Message message)
{
	if (message == CLIENT_EXIT)
	{
		printf("Recibimos CLIENT_EXIT. Saliendo...\n");
		exit(0);
	}
}

/**
 * Helper para checkear el status de funciones read
 */
Message checkMessageReceive(const ssize_t n, Message message)
{
	checkForExit(message);    // Chequeamos no haber recibido un exit
	return (n > 0) ? message : MESSAGE_FAILED;
}

Message sendMessage(const int fd, const Message message)
{
	const ssize_t n = write(fd, &message, sizeof(Message));
	return checkMessageSend(n);
}

Message receiveMessage(const int fd)
{
	Message message;
	const ssize_t n = read(fd, &message, sizeof(Message));
	return checkMessageReceive(n, message);
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