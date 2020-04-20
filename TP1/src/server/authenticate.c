#include <stdio.h>

#include "message_transmission.h"

#include "authenticate.h"


int authenticate(const int fd)
{
	Message message = receive_message(fd);
	if (message == AUTHENTICATE_REQUEST)
	{
		message = send_message(fd, AUTHENTICATE_PROCEED);
		if (message_ok(message))
		{
			Credentials credentials;
			message = receive_credentials(fd, &credentials);
			if (message_ok(message))
			{
				printf("%s %s", credentials.username, credentials.password);
			}
		}
	}
	return 1;
}