#include "authenticate.h"

int authenticate(const int fd)
{
	const enum Message authenticate_request = AUTHENTICATE_REQUEST;
	write(fd, &authenticate_request, sizeof(enum Message));
	return 1;
}