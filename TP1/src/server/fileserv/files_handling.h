#ifndef FILES_HANDLING_H

#define FILES_HANDLING_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

#include "message_transmission.h"
#include "named_pipe_connection.h"
#include "files_definitions.h"
#include "md5.h"

FileInfoDB filesDB = {.dbSize = 0u};

const size_t fullPathSize = MAX_FILENAME_LENGTH + sizeof(FILES_DIR_PATH) + 1;

void calculateFullPath(char *filePath, const char *fileName)
{
    snprintf(filePath, fullPathSize, "%s/%s", FILES_DIR_PATH, fileName);
}

bool fileExists(const char *fileName)
{
    char fullPath[fullPathSize];
    calculateFullPath(fullPath, fileName);

    return access(fullPath, F_OK) != -1;
}

bool loadDB()
{
    /* Listamos archivos del directorio de imagenes y los almacenamos en la DB */
    DIR *d  = opendir(FILES_DIR_PATH);
    bool result = d != NULL;
    if (result) {
        struct dirent *dir;
        while ((dir = readdir(d)) != NULL) {
            /* Descartamos "." y ".." */
            if (strncmp(dir->d_name, ".", MAX_FILENAME_LENGTH) != 0 &&
                strncmp(dir->d_name, "..", MAX_FILENAME_LENGTH) != 0) {
                    /* Cargamos cada elemento en la DB */
                    strncpy(filesDB.userInfo[filesDB.dbSize].fileName, dir->d_name, MAX_FILENAME_LENGTH);
                    /* Calculamos MD5 y size*/
                    char fullPath[fullPathSize];
                    calculateFullPath(fullPath, dir->d_name);

                    if (!calculateMD5(fullPath, filesDB.userInfo[filesDB.dbSize].md5Sum, &filesDB.userInfo[filesDB.dbSize].size, 0u)) {
                        printf("file: [-] No se pudo obtener MD5 y size del archivo: %s\n", fullPath);
                    }
                    filesDB.dbSize++;
            }
        }
        closedir(d);
    } else {
        printf("file: [-] No se pudo obtener contenidos del directorio de imagenes\n");
    }
    return result;
}

Message listFiles(const int serverFd)
{   
    return sendFilesDB(serverFd, &filesDB);
}

Message serveFiles(const int listenFd)
{   
    Message message = MESSAGE_FAILED;
    printf("file: [*] Levantando servidor de descargas\n");
    const int clientFd = TCPAccept(listenFd);
    if (clientFd > 0) {
        printf("file: [*] Cliente conectado\n");
        char fileName[MAX_FILENAME_LENGTH];
        message = checkMessageSend(read(clientFd, fileName, MAX_FILENAME_LENGTH));
        if (messageOk(message)) {
            printf("file: [*] Pedido para descargar %s\n", fileName);
            if (fileExists(fileName)) {
                printf("file: [*] Descarga autorizada\n");
                message = sendMessage(clientFd, FILE_DOWN_AUTHORIZED);
                if (messageOk(message)) {
                    char fullPath[fullPathSize];
                    calculateFullPath(fullPath, fileName);
                    message = sendFile(clientFd, fullPath);
                }
            } else {
                printf("file: [*] Descarga rechazada, el archivo no existe.\n");
                message = sendMessage(clientFd, FILE_DOWN_REJECTED);
            }
        }
    }
    return message;
}

#endif