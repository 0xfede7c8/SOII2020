/**
 * @file
 * @brief Funciones comunes de autenticación
 */

#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include <stdbool.h>

#include "commands.h"

/**
 * Autenticación. Cliente y servidor cada uno implementan su lado.
 *
 * @param fd file descriptor del canal
 * @return 0 o -1 dependiendo si la autenticación fue exitosa
 */
int authenticate(const int authSockFd, const int fd);


/**
 * Lista usuarios al cliente. Cliente y servidor cada uno implementan su lado.
 *
 * @param fd file descriptor del canal
 * @return MESSAGE_SUCCESS o MESSAGE_FAILED
 */
Message listUsers(const int authSockFd, const int clientFd);

/**
 * Devuelve el nombre de usuario de la ultima persona autenticada
 *
 * @return puntero constante a char con el nombre de usuario
 */
const char* getAutheticatedUsername();

#endif