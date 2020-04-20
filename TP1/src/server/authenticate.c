#include "message_transmission.h"

#include "authenticate.h"

int authenticate(const int fd)
{
	enum Message message = receive_message(fd);
	if (message == AUTHENTICATE_REQUEST)
	{
		message = send_message(fd, AUTHENTICATE_PROCEED);
	}
		
	return 1;
}