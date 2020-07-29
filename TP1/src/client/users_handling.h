#ifndef USERS_HANDLING_H

#define USERS_HANDLING_H

#include <stdio.h>

#include "message_transmission.h"

Message changePassword(const int serverFd, char *newPassword)
{
    Message message = sendMessage(serverFd, USER_PASSWORD);
    
    if (messageOk(message)) {
        /* Envíamos nueva clave */
        const ssize_t n = write(serverFd, newPassword, CREDENTIALS_SIZE);
        message = checkMessageSend(n);
        if (messageOk(message)) {
            message = receiveMessage(serverFd);
            if (message == MESSAGE_SUCCESS) {
                printf("[+] Password cambiada exitosamente\n");
            } else {
                printf("[-] Error al cambiar clave\n");
            }
        } else {
            printf("[-] Error al enviar nueva clave al servidor\n");
        }
    } 
    return message;
}

#endif