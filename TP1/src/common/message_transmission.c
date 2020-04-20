#include <unistd.h>

#include "message_transmission.h"

/**
 * Helper para checkear el status de write
 */
Message check_message_send(const ssize_t n)
{
	return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

/**
 * Helper para checkear el status de funciones read
 */
Message check_message_receive(const ssize_t n, Message message)
{
	return (n > 0) ? message : MESSAGE_FAILED;
}

Message send_message(const int fd, const Message message)
{
	const ssize_t n = write(fd, &message, sizeof(Message));
	return check_message_send(n);
}

Message receive_message(const int fd)
{
	Message message;
	const ssize_t n = read(fd, &message, sizeof(Message));
	return check_message_receive(n, message);
}

Message send_credentials(const int fd, const Credentials* credentials)
{
	const ssize_t n = write(fd, (void *)credentials, sizeof(Credentials));
	return check_message_send(n);
}

Message receive_credentials(const int fd, Credentials* credentials)
{
	const size_t credsz = sizeof(Credentials);
	const ssize_t n = read(fd, (void *)credentials, credsz);
	return (n == (ssize_t)credsz) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

int message_ok(Message message)
{
	return message == MESSAGE_SUCCESS;
}