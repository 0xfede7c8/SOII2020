#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "named_pipe_connection.h"
#include "message_transmission.h"


int checkCredentials(const Credentials* credentials)
{
    return strncmp(credentials->username, "fede", CREDENTIALS_SIZE) == 0 &&
           strncmp(credentials->password, "pepe", CREDENTIALS_SIZE) == 0;
}

int main() 
{
    int writeFd, readFd;
    Message message;
    if (getFIFOs(&readFd, &writeFd)) {
        printf("[*] auth: FIFOS creadas y conectadas\n");
        Credentials credentials;
        while (1) {
            printf("[*] auth: Recibiendo credenciales\n");
            message = receiveCredentials(readFd, &credentials);
            if (messageOk(message)) {
                if (checkCredentials(&credentials)) {
                    printf("[*] auth: Usuario y contraseña validos\n");
                    sendMessage(writeFd, AUTHENTICATE_PASSED);
                } else {
                    sendMessage(writeFd, AUTHENTICATE_FAILED);
                }
            }
        }
    } else {
        perror("[-] auth: Error al obtener las FIFOs");
    }
	return EXIT_SUCCESS;
}