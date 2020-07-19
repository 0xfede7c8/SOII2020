/**
 * @file
 * @brief Transmisión de mensajes de protocolo entre cliente y servidor
 */

#ifndef MESSAGE_TRANSMISSION_H

#define MESSAGE_TRANSMISSION_H

#include <stdbool.h>

#include "commands.h"

#define CREDENTIALS_SIZE 30
#define MAX_STRINGS_SIZE 30

/**
 * Estructura que representa las credenciales del usuario 
 */
typedef struct Credentials {
	char username[CREDENTIALS_SIZE];    /*!< nombre de usuario */
	char password[CREDENTIALS_SIZE];    /*!< contraseña */
} Credentials;

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
 * Chequea si el mensaje fue enviado o recibido correctamente
 *
 * @param message Message obtenido
 * @return 0 si no es MESSAGE_SUCCESS, 1 si lo es
 */
int messageOk(Message message);



#endif