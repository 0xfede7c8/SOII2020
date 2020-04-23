#include <stdio.h>

#include "named_pipe_connection.h"
#include "message_transmission.h"
#include "csv.h"
#include "user_authentication.h"

int main() 
{
    int writeFd, readFd;
    Message message;

    /* Cargamos credentials */
    Credentials credentialsCSV[CREDENTIAL_LIMIT_AMOUNT];
    const size_t credAmount = parseUserDataCSV(credentialsCSV, CREDENTIAL_LIMIT_AMOUNT, "bin/credentials.csv");
    if (credAmount > 0) {
        printf("[+] auth: Credenciales cargadas exitosamente\n");
        while(1) {
            if (getFIFOs(&readFd, &writeFd)) {
                printf("[*] auth: FIFOS creadas y conectadas\n");
                Credentials credentials;
                message = receiveCredentials(readFd, &credentials);
                if (messageOk(message)) {
                    printf("[*] auth: credenciales recibidas\n");
                    if (userBlocked(credentials.username))
                    {
                        printf("[*] auth: usuario bloqueado: %s\n", credentials.username);
                        sendMessage(writeFd, AUTHENTICATE_BLOCKED);
                    } else {
                        if (checkCredentials(&credentials, credentialsCSV, credAmount)) {
                            printf("[*] auth: Usuario y contraseña validos\n");
                            sendMessage(writeFd, AUTHENTICATE_PASSED);
                        } else {
                            printf("[*] auth: Usuario y contraseña invalidos\n");
                            updateUserTry(credentials.username);
                            sendMessage(writeFd, AUTHENTICATE_FAILED);
                        }
                    }
                }
            } else {
                perror("[-] auth: Error al obtener las FIFOs");
            }
        }
    } else {
        printf("[-] auth: No se parsearon credenciales. Saliendo.\n");
    }
	return EXIT_SUCCESS;
}