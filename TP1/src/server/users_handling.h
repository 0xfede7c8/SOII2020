#ifndef USERS_HANDLING_H

#define USERS_HANDLING_H

#include <stdio.h>

#include "message_transmission.h"

Message changePassword(const int authSockFd, const int clientFd)
{
    char newPasswd[CREDENTIALS_SIZE];

    /* Leemos clave del cliente */
    const ssize_t n = read(clientFd, newPasswd, CREDENTIALS_SIZE);
    Message message = checkMessageSend(n);

    if (messageOk(message)) {
        /* Pedido al auth server */
        message = sendMessage(authSockFd, USER_PASSWORD);
        if (messageOk(message)) {
            /* Enviamos nueva clave al auth server */
            const ssize_t n = write(authSockFd, newPasswd, CREDENTIALS_SIZE);
            message = checkMessageSend(n);
            if (messageOk(message)) {
                /* Recibimos estado de cambio de clave del auth server */
                message = receiveMessage(authSockFd);
                /* Lo mandamos directamente para forwardear el msg del auth server */
                message = sendMessage(clientFd, message);
            }
        }
    }
    return message;
}

#endif