/**
 * @file
 * @brief Transmisión de mensajes de protocolo entre cliente y servidor
 */

#ifndef MESSAGE_TRANSMISSION_H

#define MESSAGE_TRANSMISSION_H

#include <stdbool.h>

#include "commands.h"
#include "users_definitions.h"
#include "files_definitions.h"

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
 * Recibe una lista de strings
 *
 * @param fd file descriptor del canal
 * @param callback callback con funcion a realizar por cada string recibido
 * @return Devuelve MESSAGE_SUCCESS o MESSAGE_FAILED en caso de error
 */
Message receiveStrings(const int fd, void (*callback)(const char*));

/**
 * Envia una lista de strings
 *
 * @param fd file descriptor del canal
 * @param strings arreglo de strings a enviar
 * @param len cantidad de strings a enviar
 * @return Message segun resultado de la operacion
 */
Message sendStrings(const int fd, char* strings[], const size_t len);

/**
 * Envia una lista de usuarios
 *
 * @param fd file descriptor del canal
 * @param db puntero a UserDB con los datos a enviar
 * @return Message segun resultado de la operacion
 */
Message sendUserListFromDB(const int fd, const UserDB *db);

/**
 * Envia una DB de archivos
 *
 * @param fd file descriptor del canal
 * @param db puntero a FileInfoDB con los datos a enviar
 * @return Message segun resultado de la operacion
 */
Message sendFilesDB(const int fd, const FileInfoDB *db);

/**
 * Recibe DB de archivos
 *
 * @param fd file descriptor del canal
 * @param db puntero a FileInfoDB donde se almacenará la DB
 * @return Message segun resultado de la operacion
 */
Message receiveFilesDB(const int fd, FileInfoDB *db);


/**
 * Recibe un puerto
 *
 * @param fd file descriptor del canal
 * @param port puntero a char donde se almacenará la DB
 * @return Message segun resultado de la operacion
 */
Message receivePort(const int fd, char *port);

/**
 * Recibe un puerto
 *
 * @param fd file descriptor del canal
 * @param port puerto que se enviará
 * @return Message segun resultado de la operacion
 */
Message sendPort(const int fd, const char *port);

Message sendFile(const int fd, const char *filename);

Message receiveBootableFileAndStore(const int fd, const char *filename, size_t *amountWritten);

/**
 * Chequea si el mensaje fue enviado o recibido correctamente
 *
 * @param message Message obtenido
 * @return 0 si no es MESSAGE_SUCCESS, 1 si lo es
 */
int messageOk(Message message);



#endif