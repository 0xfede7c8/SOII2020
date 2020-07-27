#ifndef FILES_HANDLING_H

#define FILES_HANDLING_H

#include <stdio.h>
#include <unistd.h>

#include "message_transmission.h"
#include "files_definitions.h"

Message listFiles(const int filesSockFd, const int clientFd)
{
    Message message = sendMessage(filesSockFd, FILE_LIST);
    if (messageOk(message)) {
        FileInfoDB db;
        message = receiveFilesDB(filesSockFd, &db);
        if (messageOk(message)) {
            message = sendFilesDB(clientFd, &db);
        }
    }
    return message;
}

Message informPort(const int clientFd, const int filesSockFd, const char *port)
{   
    /* Avisamos al file server para que entre en modo descarga */
    Message message = sendMessage(filesSockFd, FILE_DOWN);
    /* Esperamos que el file server se ponga en modo descarga */
    sleep(3);
    if (messageOk(message)) {
        /* Devolvemos el puerto al usuario para que se conecte */
        message = sendPort(clientFd, port);
    }
    return message;
}

#endif