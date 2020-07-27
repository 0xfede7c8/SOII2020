#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include "tcp_connection.h"
#include "message_transmission.h"
#include "files_handling.h"


int main(const int argc, const char *argv[]) 
{
    if (argc != 2) {
        fprintf(stderr, "auth: [-] Uso: %s <puerto principal>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (loadDB()) {
        int listenFd;
        const int serverFd = createServerAndAccept(argv[1], &listenFd);
        if (serverFd > 0) {
            /* Bucle */
            while(1) {
                Message message = receiveMessage(serverFd);
                printf("file: [*] Mensaje recibido.\n");
                switch(message) {
                    case FILE_LIST:
                        message = listFiles(serverFd);
                        break;
                    case FILE_DOWN:
                        message = serveFiles(listenFd);
                        break;
                    default:
                        printf("file: [-] Comando recibido no identificado\n");
                        message = sendMessage(serverFd, MESSAGE_FAILED);
                }
            }
        } else {
            perror("server: [-] Problema creando servidor de archivos. Saliendo");
            exit(1);
        }        
    } else {
        printf("file: [-] No se pudo cargar la DB de imagenes\n");
    }

    return EXIT_FAILURE;
}
