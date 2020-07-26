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

#define CREDENTIALS_FILE_PATH "bin/credentials.csv"
#define MAX_TRIES 3    /*!< Máxima cantidad de usuarios intentos de autenticación */

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
        if (result != 0) {
            userDB.lastUserIndex = userIndex;
        }
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
    return sendUserListFromDB(fd, &userDB) == MESSAGE_SUCCESS;
}


void storeCSV(const UserDB *db, const char* dbPath)
{
    FILE* dbFile = fopen(dbPath, "w");

    char lineBuffer[CREDENTIALS_SIZE*3];

    uint32_t i;
    for (i=0u; i<(size_t)db->dbSize; i++) {
        snprintf(lineBuffer,
                 CREDENTIALS_SIZE*3,
                 "%s,%s\n",
                 db->userInfo[i].credentials->username,
                 db->userInfo[i].credentials->password);
        fputs(lineBuffer, dbFile);
        memset(lineBuffer, '\0', CREDENTIALS_SIZE*3);
    }
    fclose(dbFile);
}


Message changePassword(const int serverFd)
{
    char newPasswd[CREDENTIALS_SIZE];
    const ssize_t n = read(serverFd, newPasswd, CREDENTIALS_SIZE);

    Message message = checkMessageSend(n);

    if (messageOk(message)) {
        strncpy(userDB.userInfo[userDB.lastUserIndex].credentials->password, newPasswd, CREDENTIALS_SIZE);
        storeCSV(&userDB, CREDENTIALS_FILE_PATH);
        printf("auth: New password set: %s\n", newPasswd);
        message = sendMessage(serverFd, MESSAGE_SUCCESS);
    }
    return message;
}

#endif