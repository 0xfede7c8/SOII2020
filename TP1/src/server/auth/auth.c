#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "named_pipe_connection.h"
#include "message_transmission.h"
#include "csv.h"

#define CREDENTIAL_LIMIT_AMOUNT 10u

/**
 * Compara dos Credentials
 * @param cred1 primer Credentials
 * @param cred2 segundo Credentials
 * @return true si son iguales, false de otra forma 
 */
int credentialsEqual(const Credentials* cred1, const Credentials* cred2)
{
    return  strncmp(cred1->username, cred2->username, CREDENTIALS_SIZE) == 0 &&
            strncmp(cred1->password, cred2->password, CREDENTIALS_SIZE) == 0;
}

int checkCredentials(const Credentials* inputCredential, const Credentials* csvCredentials, const size_t credAmount)
{
    uint32_t i;
    for (i=0u; i<credAmount; i++) {
        if (credentialsEqual(inputCredential, csvCredentials+i)) {
            return true;
        }
    }
    return false;
}

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
                    if (checkCredentials(&credentials, credentialsCSV, credAmount)) {
                        printf("[*] auth: Usuario y contraseña validos\n");
                        sendMessage(writeFd, AUTHENTICATE_PASSED);
                    } else {
                        printf("[*] auth: Usuario y contraseña invalidos\n");
                        sendMessage(writeFd, AUTHENTICATE_FAILED);
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