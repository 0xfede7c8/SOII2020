#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "tcp_connection.h"
#include "authenticate.h"
#include "message_transmission.h"
#include "users_handling.h"

void printHelp(const int argc, char *argv[])
{
    if (argc < 2) {
            fprintf(stderr, "Uso: %s <puerto server> <ip auth server> <puerto auth server>\n", argv[0] );
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    
    printHelp(argc, argv);

    int listenFd;
    int fd = createServerAndAccept(argv[1], &listenFd);

    if (fd > 0) {
        /* Conectamos al auth server */
        const int authSockFd = TCPConnect(argv[2], argv[3]);
        if (authSockFd > 0) {
            if (authenticate(authSockFd, fd)) {
                while(1) {
                    Message message = receiveMessage(fd);
                    switch(message) {
                        case USER_LIST:
                            message = listUsers(authSockFd, fd);
                            break;
                        case USER_PASSWORD:
                            message = changePassword(authSockFd, fd);
                            break;
                        case FILE_LIST:
                            printf("filelist\n");
                            break;
                        case FILE_DOWN:
                            printf("filedown\n");
                            break;
                        case CLIENT_EXIT:
                            close(fd);
                            printf("server: [*] Usuario desconectado\n");
                            fd = TCPAccept(listenFd);
                            printf("server: [+] Nuevo usuario conectado. Autenticando..\n");
                            authenticate(authSockFd, fd);
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
        } else {
                printf("[-] server: No se pudo establecer comunicación con el auth server\n");
        }
    close(fd);
    } else {
        perror("server: [-] Problema creando servidor. Saliendo");
        exit(1);
    }
    return EXIT_SUCCESS; 
} 
