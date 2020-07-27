#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "message_transmission.h"
#include "tcp_connection.h"

#define MAX_MESSAGE_LENGTH 1024

Message sendMessage(const int fd, const Message message)
{
    const ssize_t n = write(fd, &message, sizeof(Message));
    return checkMessageSend(n);
}

Message receiveMessage(const int fd)
{   
    const size_t msgSize = sizeof(Message);
    Message message = MESSAGE_FAILED;
    
    ssize_t n = 0u;
    while (n == 0) {
        /* Según el largo decidimos si el mensaje recibido es un comando 
           y lo almacenamos o no */
        n = read(fd, (void *)&message, msgSize);
        if (n != msgSize) {
            /* No deberiamos recibir más que el tamaño de Message.
               Si esto ocurre es que estamos tratando de recibir un comando
               cuando el mensaje en realidad es más largo. En definitiva un bug
               en la implementación del protocolo */
            message = MESSAGE_FAILED;
        } 
    }

    return message;
}

Message sendCredentials(const int fd, const Credentials* credentials)
{
    const ssize_t n = write(fd, (void *)credentials, sizeof(Credentials));
    return checkMessageSend(n);
}

Message receiveCredentials(const int fd, Credentials* credentials)
{
    const size_t credsz = sizeof(Credentials);
    const ssize_t n = read(fd, (void *)credentials, credsz);
    return (n == (ssize_t)credsz) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

int messageOk(Message message)
{
    return message == MESSAGE_SUCCESS;
}

Message sendUser(const int fd, const char *username)
{
    /* Avisamos que tenemos un usuario para enviar */
    Message status = sendMessage(fd, USER_LIST);
    if (status) {
            /* Enviamos el usuario */
            const ssize_t n = write(fd, username, MAX_STRINGS_SIZE);
            status = checkMessageSend(n);
    }
    return status;
}

Message sendUserListFromDB(const int fd, const UserDB *db)
{
    Message status = MESSAGE_SUCCESS;
    size_t i = 0;
    size_t len = db->dbSize;

    while( (i<len) && (status == MESSAGE_SUCCESS))
    {
        status = sendUser(fd, db->userInfo[i].credentials->username);
        i++;
    }
    /* Cuando terminamos de mandar avisamos que ya no hay mas strings */
    if (status == MESSAGE_SUCCESS)
    {
        status = sendMessage(fd, USER_LIST_FINISH);
    } else {
        perror("[-] Error mandando lista de usuarios");
    }
    return status;
}

Message sendStrings(const int fd, char* strings[], const size_t len)
{
    Message status = MESSAGE_SUCCESS;
    uint32_t i = 0;
    while( (i<len) && (status == MESSAGE_SUCCESS))
    {
        status = sendUser(fd, strings[i]);
        i++;
    }
    /* Cuando terminamos de mandar avisamos que ya no hay mas strings */
    if (status == MESSAGE_SUCCESS)
    {
        status = sendMessage(fd, USER_LIST_FINISH);
    } else {
        perror("[-] Error mandando lista de usuarios");
    }
    return status;
}

Message receiveStrings(const int fd, void (*callback)(const char*))
{
    Message status;
    char user[MAX_STRINGS_SIZE];

    bool finish = false;

    while (!finish) {
        status = receiveMessage(fd);
        if (status == USER_LIST) {
            ssize_t n = 0u;
            /* Esperamos hasta que haya mensaje */
            while (n <= 0) {
                n = read(fd, (void *)user, MAX_STRINGS_SIZE);
                if (n > 0) {
                    callback(user);
                }
            }
        }
        finish = status == USER_LIST_FINISH || 
                 status == MESSAGE_FAILED;
    }

    return (status == USER_LIST_FINISH) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

Message sendFilesDB(const int fd, const FileInfoDB *db)
{
    return checkMessageSend(write(fd, db, sizeof(FileInfoDB)));
}

Message receiveFilesDB(const int fd, FileInfoDB *db)
{
    const ssize_t n = read(fd, db, sizeof(FileInfoDB));
    return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

Message receivePort(const int fd, char *port)
{
    const ssize_t n = read(fd, port, PORT_MAX_LENGTH);
    return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

Message sendPort(const int fd, const char *port)
{
    return checkMessageSend(write(fd, port, PORT_MAX_LENGTH));
}

Message sendFile(const int clientFd, const char *filePath)
{   
    char buff[MAX_MESSAGE_LENGTH];  // to store message from client
 
    FILE *fp = fopen(filePath, "rb"); 

    size_t totalSent = 0u;
    if (fp != NULL) {
        size_t n = fread(buff, MAX_MESSAGE_LENGTH, 1, fp); 
        while (n > 0) {
            n = (size_t)write(clientFd, buff, MAX_MESSAGE_LENGTH);
            totalSent += n;
            n = fread(buff, MAX_MESSAGE_LENGTH, 1, fp); 
        }
        close(clientFd);
        fclose(fp);
    }
    return (totalSent > 0u) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

Message receiveFile(const int serverFd, const char *filePath)
{   
    char buff[MAX_MESSAGE_LENGTH];  // to store message from client
 
    FILE *fp = fopen(filePath, "wb"); 

    ssize_t totalReceived = 0u;
    if (fp != NULL) {
        ssize_t n = read(serverFd, buff, MAX_MESSAGE_LENGTH);
        while (n > 0) {
            fwrite(buff, (size_t)n, 1, fp);
            totalReceived += n;
            n = read(serverFd, buff, MAX_MESSAGE_LENGTH); 
        }
        close(serverFd);
        fclose(fp);
    }
    return (totalReceived > 0u) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}
