/**
 * @file
 * @brief Transmisión de mensajes de protocolo entre cliente y servidor
 */

#ifndef MESSAGE_TRANSMISSION_H

#define MESSAGE_TRANSMISSION_H

#include <stdbool.h>

#include "commands.h"
#include "users_definitions.h"

/**
 * Helper para checkear el status de write
 */
inline Message checkMessageSend(const ssize_t n)
{
    return (n > 0) ? MESSAGE_SUCCESS : MESSAGE_FAILED;
}

/**
 * Envía comando por el fd especificado
 *
 * @param fd file descriptor del canal
 * @return Devuelve mensaje MESSAGE_SUCCESS o MESSAGE_FAILED
 */
Message sendMessage(const int fd, Message message);

/**
 * Recibe comando por el fd especificado
 *
 * @param fd file descriptor del canal
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
Message receiveMessage(const int fd);


/**
 * Envía las credenciales por el fd especificado
 *
 * @param fd file descriptor del canal
 * @param credentials puntero a estructura Credentials donde se obtendrán las credenciales a enviar
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
Message sendCredentials(const int fd, const Credentials* credentials);

/**
 * Recibe las credenciales por el fd especificado
 *
 * @param fd file descriptor del canal
 * @param credentials puntero a estructura Credentials donde se devolverán las credenciales
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
Message receiveCredentials(const int fd, Credentials* credentials);

/**
 * Recibe una lista de usuarios
 *
 * @param fd file descriptor del canal
 * @param callback callback con funcion a realizar por cada string recibido
 * @return Devuelve MESSAGE_SUCCESS o MESSAGE_FAILED en caso de error
 */
Message receiveUserList(const int fd, void (*callback)(const char*));

/**
 * Envia una lista de usuarios
 *
 * @param fd file descriptor del canal
 * @param strings arreglo de strings a enviar
 * @param len cantidad de strings a enviar
 * @return Message segun resultado de la operacion
 */
Message sendUserList(const int fd, char* strings[], const size_t len);

/**
 * Envia una lista de usuarios
 *
 * @param fd file descriptor del canal
 * @param db puntero a UserDB con los datos a enviar
 * @return Message segun resultado de la operacion
 */
Message sendUserListFromDB(const int fd, const UserDB *db);

/**
 * Chequea si el mensaje fue enviado o recibido correctamente
 *
 * @param message Message obtenido
 * @return 0 si no es MESSAGE_SUCCESS, 1 si lo es
 */
int messageOk(Message message);



#endif