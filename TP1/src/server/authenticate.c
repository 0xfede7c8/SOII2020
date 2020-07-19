#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "message_transmission.h"
#include "tcp_connection.h"
#include "authenticate.h"

/**
 * Checkear credenciales contra el servidor de autenticación (mover a su propio .h)
 *
 * @param credentials estructura Credentials donde estan almacenadas las credenciales a checkear
 * @return AUTHENTICATE_PASSED, AUTHENTICATE_FAILED, o AUTHENTICATION_BLOCKED dependiendo del estado 
 */
Message checkAgainstAuthServer(const int authSockFd, const Credentials* credentials) 
{
	Message message = AUTHENTICATE_FAILED;

	printf("server: [*] Enviando autorización para autenticar\n");
	message = sendMessage(authSockFd, AUTHENTICATE_REQUEST);
	if (messageOk(message)) {
		/* Recibimos mensaje de sincronización */
		message = receiveMessage(authSockFd);
		if (message == AUTHENTICATE_PROCEED) {
			printf("server: [*] Enviando credenciales al auth server\n");
			message = sendCredentials(authSockFd, credentials);
			if (messageOk(message))
			{
				/* Recibimos autenticación o no del auth server */ 
				message = receiveMessage(authSockFd);
			}
		}
	}
	return message;
}

int authenticate(const int authSockFd, const int clientFd)
{
	bool authenticated = false;
	while(!authenticated) {
		Message message = receiveMessage(clientFd);
		if (message == AUTHENTICATE_REQUEST) {
			printf("server: [*] Recibimos pedido de autenticacion\n");
			message = sendMessage(clientFd, AUTHENTICATE_PROCEED);
			if (messageOk(message)) {
				Credentials credentials;
				message = receiveCredentials(clientFd, &credentials);
				if (messageOk(message)) {
					printf("server: [*] Usuario: %s\n", credentials.username);
					printf("server: [*] Contraseña: %s\n", credentials.password);
					message = checkAgainstAuthServer(authSockFd, &credentials); 
					if (message == AUTHENTICATE_PASSED) {
						authenticated = true;
						message = sendMessage(clientFd, AUTHENTICATE_PASSED);
						printf("server: [+] Autenticación exitosa\n");
					} else if (message == AUTHENTICATE_FAILED) {
						printf("server: [-] Autenticación fallida\n");
						message = sendMessage(clientFd, AUTHENTICATE_FAILED);
					} else if (message == AUTHENTICATE_BLOCKED) {
						printf("server: [-] Usuario bloqueado: %s\n", credentials.username);
						message = sendMessage(clientFd, AUTHENTICATE_BLOCKED);
					} else {
						printf("server: [-] Error del auth server. Problema de FIFO, o protocolo\n");
						message = sendMessage(clientFd, MESSAGE_FAILED);
						return authenticated;
					}
				}
			}
		}
	}
	return authenticated;
}

#define CREDENTIAL_LIMIT_AMOUNT 10u

/* Guardamos los usernames recibidos para reenviar al cliente */
char *usernames[CREDENTIAL_LIMIT_AMOUNT];
static uint32_t usernameIndex = 0u;

void storeUsername(const char* username)
{
	usernames[usernameIndex] = malloc(CREDENTIALS_SIZE*sizeof(char));
	char *usernamePtr = usernames[usernameIndex];
	if (usernamePtr != NULL) {
		memset(usernamePtr, '\0', CREDENTIALS_SIZE);
		strncpy(usernamePtr, username, CREDENTIALS_SIZE);
	}
	usernameIndex++;
}

Message listUsers(const int authSockFd, const int clientFd)
{	
	Message message = MESSAGE_FAILED;

	/* Pedimos al authserver la lista de usuarios */
	message = sendMessage(authSockFd, USER_LIST);

	if (messageOk(message)) {
		/* Recibimos la lista de usuarios desde el auth server */
		message = receiveUserList(authSockFd, storeUsername);
		if (messageOk(message)) {
			/* Enviamos la lista de usuarios al cliente */
			message = sendUserList(clientFd, usernames, usernameIndex);
		}
	}

	/* Liberamos el buffer de usernames */
	uint32_t i;
	for (i = 0u; i < usernameIndex; i++) {
		free(usernames[i]);
	}
	usernameIndex = 0;
	
	return message;
}
