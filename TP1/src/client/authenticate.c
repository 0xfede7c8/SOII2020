#include <stdio.h>

#include "message_transmission.h"

#include "authenticate.h"

int authenticate(const int fd)
{
	// Mandamos solicitud para autenticar
	enum Message message = send_message(fd, AUTHENTICATE_REQUEST);
	if (message == MESSAGE_SUCCESS) {
		message = receive_message(fd);
		if (message == AUTHENTICATE_PROCEED)
		{
			printf("me procededieron");
		}
	}
	return 1;
}