#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"
#include "message_transmission.h"
#include "users_handling.h"
#include "files_handling.h"

void printHelp(const int argc, char *argv[])
{
    if (argc != 5) {
            fprintf(stderr, "Uso: %s <puerto server> <ip servicios> <puerto auth server> <puerto files server>\n", argv[0] );
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    
    printHelp(argc, argv);

    int listenFd;
    int clientFd = createServerAndAccept(argv[1], &listenFd);

    if (clientFd > 0) {
        /* Conectamos al auth server */
        const int authSockFd = TCPConnect(argv[2], argv[3]);
        /* Conectamos al files server */
        const int filesSockFd = TCPConnect(argv[2], argv[4]);
        if (authSockFd > 0) {
            if (authenticate(authSockFd, clientFd)) {
                while(1) {
                    Message message = receiveMessage(clientFd);
                    switch(message) {
                        case USER_LIST:
                            message = listUsers(authSockFd, clientFd);
                            break;
                        case USER_PASSWORD:
                            message = changePassword(authSockFd, clientFd);
                            break;
                        case FILE_LIST:
                            message = listFiles(filesSockFd, clientFd);
                            break;
                        case GET_FILE_SERVER_PORT:
                            message = informPort(clientFd, filesSockFd, argv[4]);
                            break;
                        case CLIENT_EXIT:
                            close(clientFd);
                            printf("server: [*] Usuario desconectado\n");
                            clientFd = TCPAccept(listenFd);
                            printf("server: [+] Nuevo usuario conectado. Autenticando..\n");
                            authenticate(authSockFd, clientFd);
                            break;
                        case MESSAGE_FAILED:
                            /* No hay nada que leer */
                            break;
                        default:
                            printf("server: [-] Comando recibido no identificado\n");
                    }
                }
            } 
        close(authSockFd);
        close(filesSockFd);
        } else {
                printf("[-] server: No se pudo establecer comunicación con el auth server\n");
        }
    close(clientFd);
    } else {
        perror("server: [-] Problema creando servidor. Saliendo");
        exit(1);
    }
    return EXIT_SUCCESS; 
} 
