#include <stdio.h>

#include "message_transmission.h"

#include "authenticate.h"

int authenticate(const int fd)
{
	// Mandamos solicitud para autenticar
	Message message = send_message(fd, AUTHENTICATE_REQUEST);
	if (message_ok(message)) {
		message = receive_message(fd);
		if (message == AUTHENTICATE_PROCEED)
		{
			Credentials credentials = {"user", "password"};
			message = send_credentials(fd, &credentials);
			if (message_ok(message))
			{
				printf("mensaje enviado");
			}
		}
	}
	return 1;
}