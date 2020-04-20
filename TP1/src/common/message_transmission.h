#ifndef MESSAGE_TRANSMISSION_H

#define MESSAGE_TRANSMISSION_H

#include "commands.h"

#define CREDENTIALS_SIZE 30

/**
 * Estructura que representa las credenciales del usuario 
 */
struct Credentials {
	char username[CREDENTIALS_SIZE];    /*!< nombre de usuario */
	char password[CREDENTIALS_SIZE];    /*!< contraseña */
};

/**
 * Envía comando por el fd especificado
 *
 * @param fd file descriptor del canal
 * @return Devuelve mensaje MESSAGE_SUCCESS o MESSAGE_FAILED
 */
enum Message send_message(const int fd, enum Message message);

/**
 * Recibe comando por el fd especificado
 *
 * @param fd file descriptor del canal
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
enum Message receive_message(const int fd);


/**
 * Envía las credenciales por el fd especificado
 *
 * @param fd file descriptor del canal
 * @param credentials estructura Credentials donde se guardarán las credenciales
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
enum Message send_credentials(const int fd, const struct Credentials credentials);

/**
 * Recibe las credenciales por el fd especificado
 *
 * @param fd file descriptor del canal
 * @param credentials estructura Credentials donde se guardarán las credenciales
 * @return Devuelve el mensaje recibido o MESSAGE_FAILED en caso de error
 */
enum Message receive_credentials(const int fd, const struct Credentials credentials);



#endif