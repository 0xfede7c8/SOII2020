/**
 * @file
 * @brief Funciones de autenticación de usuario
 */
#ifndef USER_AUTHENTICATION_H

#define USER_AUTHENTICATION_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "message_transmission.h"

#define CREDENTIAL_LIMIT_AMOUNT 10u    /*!< Máxima cantidad de usuarios registrados */
#define MAX_TRIES 3                    /*!< Máxima cantidad de usuarios intentos de autenticación */

/**
 * Estructura que representa la cantidad de intentos por nombre de usuario 
 */
typedef struct UserInfo {
    Credentials* credentials;
    char* lastLoginTime;
    uint32_t triesCount;
} UserInfo;

/**
 * Estructura que representa las base de datos
 * de los intentos realizados por cada usuario
 */
typedef struct UserDB {
    UserInfo userInfo[CREDENTIAL_LIMIT_AMOUNT];  /*!< DB de intentos */
    size_t dbSize;                               /*!< Tamaño de la base de datos */
} UserDB;

UserDB userDB = {.dbSize = 0u};

/**
 * Carga el arreglo de credenciales obtenido del CSV parseado a la base de datos de usuario
 * @param credentials puntero al arreglo de credentials
 * @param credSize tamaño de las credentials
 * @return cantidad de credentials cargadas 
 */
int loadUserDBFromCredentials(Credentials* credentials, const size_t credSize) {
    int i;
    for (i=0u; i<(int)credSize; i++) {
        userDB.userInfo[i].credentials = (credentials+i);
    }
    if (credSize > 0) {
        userDB.dbSize = (size_t)i;
    }
    return i;
}

/**
 * Compara dos Credentials
 * @param cred1 primer Credentials
 * @param cred2 segundo Credentials
 * @return true si son iguales, false de otra forma 
 */
int credentialsEqual(const Credentials* cred1, const Credentials* cred2)
{
    return  strncmp(cred1->username, cred2->username, CREDENTIALS_SIZE) == 0 &&
            strncmp(cred1->password, cred2->password, CREDENTIALS_SIZE) == 0;
}

/**
 * Busca un usuario en la base de datos de intentos
 * @param username nombre de usuario
 * @return devuelve el índice dentro de la base de datos del usuario elegido
 *         o -1 si no se encuentra
 */
int findUser(const char* username)
{
    int i;
    for (i = 0; (size_t)i<userDB.dbSize; i++) {
        if (strncmp(userDB.userInfo[i].credentials->username, username, CREDENTIALS_SIZE) == 0) {
            return i;
        }
    }
    return -1;
}

int checkCredentials(const Credentials* inputCredential)
{
    bool result = false;
    
    int userIndex = findUser(inputCredential->username);
    if (userIndex != -1)
    {
        result = credentialsEqual(inputCredential, userDB.userInfo[userIndex].credentials);
    }
    return result;
}


/**
 * Actualiza el contador de intentos de un usuario en particular
 * @param username nombre de usuario
 */
void updateUserTry(const char* username)
{   
    /* Buscamos en el array de intentos si existe el usuario */
    int userIndex = findUser(username);
    if (userIndex != -1) {
        /* Si existe, aumentamos su contador */
        userDB.userInfo[userIndex].triesCount++;
    } else {
        printf("auth: [-] Usuario no encontrado en la DB: %s. No se actualiza contador\n", username);
    }
}
/**
 * Chequea si un usuario esta bloqueado y actualiza la base de 
 * datos de intentos
 * @param username nombre de usuario
 * @return true si el usuario esta bloqueado, false de otra forma 
 */
bool userBlocked(const char* username) {
    const int userIndex = findUser(username);
    if (userIndex != -1) {
        return userDB.userInfo[userIndex].triesCount >= MAX_TRIES;
    }
    return false;
}


bool listUsers(const int fd)
{
    /* Guardamos los usernames recibidos para reenviar al cliente */
    char *userlist[CREDENTIAL_LIMIT_AMOUNT];
    
    /* Alocamos nuevo arreglo con los datos a enviar */
    uint32_t i;
    for (i = 0u; i < userDB.dbSize; i++) {
        userlist[i] = malloc(CREDENTIALS_SIZE*sizeof(char));
        char *usernamePtr = userlist[i];
        if (usernamePtr != NULL) {
            memset(usernamePtr, '\0', CREDENTIALS_SIZE);
            strncpy(usernamePtr, userDB.userInfo->credentials->username , CREDENTIALS_SIZE);
        }
    }

    const bool result = sendUserList(fd, userlist, i) == MESSAGE_SUCCESS;

    /* Liberamos el buffer de usernames */
    for (i = 0u; i < userDB.dbSize; i++) {
        free(userlist[i]);
    }

    return result;
}

Message changePassword(const int serverFd)
{
    char newPasswd[CREDENTIALS_SIZE];
    const ssize_t n = read(serverFd, newPasswd, CREDENTIALS_SIZE);

    Message message = checkMessageSend(n);

    if (messageOk(message)) {

        // TODO: REALMENTE CAMBIAR CLAVE DE USUARIO ACA
        printf("auth: %s\n", newPasswd);
        message = sendMessage(serverFd, MESSAGE_SUCCESS);
    }
    return message;
}

#endif