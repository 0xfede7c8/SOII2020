/**
 * @file
 * @brief Funciones comunes de autenticación
 */

#ifndef AUTHENTICATE_H
#define AUTHENTICATE_H

#include "commands.h"

/**
 * Autenticación. Cliente y servidor cada uno implementan su lado.
 *
 * @param fd file descriptor del canal
 * @return 0 o -1 dependiendo si la autenticación fue exitosa
 */
int authenticate(int fd);

/**
 * Devuelve el nombre de usuario de la ultima persona autenticada
 *
 * @return puntero constante a char con el nombre de usuario
 */
char* getAutheticatedUsername();

#endif