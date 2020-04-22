#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "message_transmission.h"
#include "named_pipe_connection.h"

#include "authenticate.h"

/**
 * Checkear credenciales contra el servidor de autenticación (mover a su propio .h)
 *
 * @param 
 * @param 
 * @return 
 */
int checkAgainstAuthServer(const Credentials* credentials) 
{
	int readFd, writeFd;
	bool result = false;
	Message message;
	if (getFIFOs(&readFd, &writeFd)) {
		message = sendCredentials(writeFd, credentials);
		if (messageOk(message))
		{
			printf("[*] Credenciales recibidas por el auth server\n");
			message = receiveMessage(readFd);
			result = message == AUTHENTICATE_PASSED; // todo
		}
	} else {
		printf("[-] No se pudo establecer comunicación con el auth server\n");
	}
	return result;
}


int authenticate(const int fd)
{
	bool authenticated = false;
	while(!authenticated) {
		Message message = receiveMessage(fd);
		if (message == AUTHENTICATE_REQUEST) {
			printf("[*] Recibimos pedido de autenticacion\n");
			printf("[*] Enviando autorización para autenticar\n");
			message = sendMessage(fd, AUTHENTICATE_PROCEED);
			if (messageOk(message)) {
				Credentials credentials;
				message = receiveCredentials(fd, &credentials);
				if (messageOk(message)) {
					printf("[*] Usuario: %s\n", credentials.username);
					printf("[*] Contraseña: %s\n", credentials.password);
					printf("[*] Enviando credenciales al auth server\n");
					if (checkAgainstAuthServer(&credentials)) {
						authenticated = true;
						message = sendMessage(fd, AUTHENTICATE_PASSED);
						printf("[+] Autenticación exitosa\n");
					}
					else {
						printf("[-] Autenticación fallida\n");
						message = sendMessage(fd, AUTHENTICATE_FAILED);
					}
				}
			}
		}
	}
	return authenticated;
}