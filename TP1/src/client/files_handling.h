#ifndef FILES_HANDLING_H

#define FILES_HANDLING_H

#include <stdio.h>
#include <unistd.h>

#include "message_transmission.h"
#include "files_definitions.h"
#include "md5.h"
#include "mbr.h"

#define RETRY_TIMES 3u

/**
 * Se comunica con el servidor para obtener la lista de archivos
 *
 * @param serverFd fd con conexión al servidor
 * @return Message con resultado de la operación
 */
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
                printMD5Digest(db.userInfo[i].md5Sum);
            }
        }
    }
    return message;
}

/**
 * Imprime información sobre las particiones del archivo especificado
 *
 * @param devicePath path al dispositivo o archivo a analizar
 * @param size tamaño del archivo
 */
void printStats(const char *devicePath, const size_t size)
{   
    size_t sizeOfData;
    unsigned char digest[MD5_DIGEST_LENGTH];
    if (calculateMD5(devicePath, digest, &sizeOfData, size)) {
        printf("MD5 checksum:");
        printMD5Digest(digest);
        printMBRInfo(devicePath);
    }
}

/**
 * Trata de conectarse al servidor de archivos y reintenta de no ser posible
 *
 * @param ip ip a conectarse
 * @param port puerto a conectarse
 * @return file descriptor
 */
int connectToFilesServer(const char *ip, const char *port)
{
    uint32_t retries = 0u;
    int fd = 0;
    while (fd <= 0 && retries <= RETRY_TIMES) {
        fd = TCPConnect(ip, port);
        retries++;
        sleep(1);
    }
    return fd;
}


/**
 * Protocolo para la descarga de archivo desde el fileserver
 *
 */
Message getFile(const int serverFd, const char *ip, const char *filename, const char *deviceName)
{   
    /* Pedimos al server que nos de el puerto del file server */
    Message message = sendMessage(serverFd, GET_FILE_SERVER_PORT);
    if (messageOk(message)) {
        char port[PORT_MAX_LENGTH];
        /* El servidor nos devuelve el puerto al cual comunicarnos para descargar el archivo */
        message = receivePort(serverFd, port);
        if (messageOk(message)) {

            const int fileSockFd = connectToFilesServer(ip, port);
            
            if (fileSockFd > 0) {
                /* Enviamos al file server el nombre del archivo a descargar */
                message = checkMessageSend(write(fileSockFd, filename, MAX_FILENAME_LENGTH));
                if (messageOk(message)) {
                    size_t amountWritten;
                    /* File server nos dice si se puede descargar */
                    message = receiveMessage(fileSockFd);
                    switch(message) {
                        case FILE_DOWN_AUTHORIZED:
                            printf("[*] Recibiendo y grabando imágen...\n");
                            message = receiveBootableFileAndStore(fileSockFd, deviceName, &amountWritten);
                            if (messageOk(message)) {
                                printf("[+] Imágen grabada exitosamente.\n");
                                printStats(deviceName, amountWritten);
                            }
                            break;
                        case FILE_DOWN_REJECTED:
                            printf("[-] No se puede descargar la imágen. Archivo inexistente.\n");
                            break;
                        default:
                            printf("[-] Error con el servidor de archivos.\n");
                    }
                    close(fileSockFd);
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