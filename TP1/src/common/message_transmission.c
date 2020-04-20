#include <unistd.h>

#include "message_transmission.h"

enum Message send_message(const int fd, const enum Message message)
{
	const ssize_t n = write(fd, &message, sizeof(enum Message));
	return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

enum Message receive_message(const int fd)
{
	enum Message message;
	const ssize_t n = read(fd, &message, sizeof(enum Message));
	return (n > 0) ? message : MESSAGE_FAILED;
}