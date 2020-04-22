#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include "message_transmission.h"

#include "authenticate.h"

char lastAuthenticated[CREDENTIALS_SIZE];

char* getAutheticatedUsername()
{
	return lastAuthenticated;
}

Message tryToAuthenticate(const int fd, const Credentials* credentials)
{
	Message message = sendMessage(fd, AUTHENTICATE_REQUEST);
	if (messageOk(message)) {
		message = receiveMessage(fd);
		if (message == AUTHENTICATE_PROCEED)
		{
			message = sendCredentials(fd, credentials);
			if (messageOk(message))
			{
				message = receiveMessage(fd);
			}
		}
	}
	return message;
}

int authenticate(const int fd)
{
	Message message = AUTHENTICATE_FAILED;
	Credentials credentials;
	bool finish = false;

	while ((message != AUTHENTICATE_PASSED) && !finish) {
		/* Inicializamos el usuario y contraseña a \0 */
		memset(credentials.username, '\0', CREDENTIALS_SIZE);
		memset(credentials.password, '\0', CREDENTIALS_SIZE);

		printf("Nombre de usuario: ");
		// fgets pude fallar
		if (fgets(credentials.username, CREDENTIALS_SIZE-1, stdin) == NULL) {
			return false;
		}

		const char* password = getpass("Contraseña: ");
		strncpy(credentials.password, password, CREDENTIALS_SIZE-1);

		// Borramos los \n del input de fgets
		credentials.username[strcspn(credentials.username, "\n")] = 0;
		credentials.password[strcspn(credentials.password, "\n")] = 0;

		// Mandamos solicitud para autenticar
		message = tryToAuthenticate(fd, &credentials);
		switch(message) {
			case AUTHENTICATE_BLOCKED:
				printf("[-] Autenticación bloqueada para el usuario: %s\n", credentials.username);
				printf("[*] Pruebe con otro usuario\n");
				break;
			case AUTHENTICATE_FAILED:
				printf("[-] Usuario o contraseña incorrecto. Pruebe nuevamente.\n");
				break;
			case AUTHENTICATE_PASSED:
				printf("\n[+] Autenticación exitosa. Bienvenido %s!\n", credentials.username);
				// Guardamos el nombre 
				strncpy(lastAuthenticated, credentials.username, CREDENTIALS_SIZE);
				break;
			default:    // Nos vino otro código que no handleamos . Salimos
				finish = true;
				break;
		}
	}
	return message == AUTHENTICATE_PASSED;
}