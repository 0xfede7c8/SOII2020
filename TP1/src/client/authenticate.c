#include "authenticate.h"

int authenticate(const int fd)
{
	// Mandamos solicitud para autenticar
	const enum Message authenticate_request = AUTHENTICATE_REQUEST;
	write(fd, (const void*) &authenticate_request, sizeof(enum Message));

	return 1;
}