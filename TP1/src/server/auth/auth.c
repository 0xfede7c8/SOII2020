#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "tcp_connection.h"
#include "message_transmission.h"
#include "csv.h"
#include "user_authentication.h"
#include "named_pipe_connection.h"


void authenticate(const int fd)
{
    /* Enviamos mensaje de sincronización */
    Message message = sendMessage(fd, AUTHENTICATE_PROCEED);

    if (messageOk(message)) {
        Credentials credentials;
        message = receiveCredentials(fd, &credentials);
        if (messageOk(message)) {
            printf("auth: [+] credenciales recibidas -> username: %s - password: %s\n", credentials.username, credentials.password);
            if (userBlocked(credentials.username)) {
                printf("auth: [*] usuario bloqueado: %s\n", credentials.username);
                sendMessage(fd, AUTHENTICATE_BLOCKED);
            } else {
                if (checkCredentials(&credentials)) {
                    printf("auth: [*] Usuario y contraseña validos\n");
                    sendMessage(fd, AUTHENTICATE_PASSED);
                } else {
                    printf("auth: [*] Usuario y/o contraseña invalidos\n");
                    updateUserTry(credentials.username);
                    sendMessage(fd, AUTHENTICATE_FAILED);
                }
            }
        } else {
            printf("[-] auth: no se pudo recibir credenciales\n");
        }
    } else {
        printf("[-] auth: no se pudo enviar AUTHENTICATE_PROCEED\n");
    }
}

int main(const int argc, const char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "auth: [-] Uso: %s <numero de puerto>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int result = EXIT_FAILURE;

    /* Cargamos credenciales */
    Credentials credentialsCSV[CREDENTIAL_LIMIT_AMOUNT];
    const size_t credAmount = parseUserDataCSV(credentialsCSV, CREDENTIAL_LIMIT_AMOUNT, CREDENTIALS_FILE_PATH);
    
    if (credAmount > 0) {
        /* Las pasamos a la DB para ser usadas por el servicio de autenticación */
        loadUserDBFromCredentials(credentialsCSV, credAmount);
        printf("auth: [+] Credenciales cargadas exitosamente\n");
        
        if (createFIFOs()) {
            int listenFd;
            const int fd = createServerAndAccept(argv[1], &listenFd);

            if (fd > 0) {
                /* Bucle */
                while(1) {
                    Message message = receiveMessage(fd);
                    printf("auth: [*] Mensaje recibido.\n");
                    switch(message) {
                        /* Pedido de autenticación */
                        case AUTHENTICATE_REQUEST:
                            authenticate(fd);
                            break;
                        case USER_LIST:
                            if (!listUsers(fd)) {
                                printf("auth: [-] No se pudo listar usuarios\n");
                            }
                            break;
                        case USER_PASSWORD:
                            changePassword(fd);
                            break;
                        default:
                            printf("auth: [-] Comando recibido no identificado\n");
                            message = sendMessage(fd, MESSAGE_FAILED);
                    }
                }
            } else {
                perror("server: [-] Problema creando servidor de autenticación. Saliendo");
            }        
        } else {
            perror("server: [-] Problema creando FIFOs. Saliendo");
        }
    } else {
        printf("auth: [-] No se parsearon credenciales. Saliendo.\n");
    }

	return result;
}
