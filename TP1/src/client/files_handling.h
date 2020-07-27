#ifndef FILES_HANDLING_H

#define FILES_HANDLING_H

#include <stdio.h>

#include "message_transmission.h"
#include "files_definitions.h"
#include "md5.h"

#define TMP_FILE_PATH "bin/file_download/download.iso"

Message listFiles(const int serverFd)
{
    Message message = sendMessage(serverFd, FILE_LIST);
    if (messageOk(message)) {
        FileInfoDB db;
        message = receiveFilesDB(serverFd, &db);
        if (messageOk(message)) {
            printf("Imagen | Tamaño | MD5\n");
            size_t i;
            for (i = 0u; i < db.dbSize; i++) {
                printf("%s | %lu | ", db.userInfo[i].fileName,
                                      db.userInfo[i].size);
                size_t j;
                /* Transformamos el digesto MD5 en ASCII */
                for (j=0u; j<MD5_DIGEST_LENGTH; j++)
                {
                    printf("%02x", db.userInfo[i].md5Sum[j]);
                }
                printf("\n");
            }
        }
    }
    return message;
}

Message getFile(const int serverFd, const char* ip, char *filename)
{   
    /* Pedimos al server que nos de el puerto del file server */
    Message message = sendMessage(serverFd, GET_FILE_SERVER_PORT);
    if (messageOk(message)) {
        char port[PORT_MAX_LENGTH];
        /* El servidor nos devuelve el puerto al cual comunicarnos para descargar el archivo */
        message = receivePort(serverFd, port);
        if (messageOk(message)) {
            const int fileSockFd = TCPConnect(ip, port);
            if (fileSockFd > 0) {
                /* Enviamos al file server el nombre del archivo a descargar */
                message = checkMessageSend(write(fileSockFd, filename, MAX_FILENAME_LENGTH));
                if (messageOk(message)) {
                    printf("[*] Recibiendo imágen...\n");
                    message = receiveFile(fileSockFd, TMP_FILE_PATH);
                    printf("[*] Imágen recibida exitosamente.\n");
                }
            } else {
                printf("[-] No se pudo conectar al files server\n");
                message = MESSAGE_FAILED;
            }
        }
    }
    return message;
}

#endif