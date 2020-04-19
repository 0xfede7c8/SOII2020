#include <stdio.h>

#include "authenticate.h"

int authenticate(const int fd)
{
	enum Message authenticate_request;
	read(fd, &authenticate_request, sizeof(enum Message));
	if (authenticate_request != AUTHENTICATE_REQUEST) {
		printf("Not request");
	}
	return 1;
}