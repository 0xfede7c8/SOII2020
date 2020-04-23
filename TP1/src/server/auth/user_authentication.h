/**
 * @file
 * @brief Funciones de autenticación de usuario
 */
#ifndef USER_AUTHENTICATION_H

#define USER_AUTHENTICATION_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#include "message_transmission.h"

#define CREDENTIAL_LIMIT_AMOUNT 10u    /*!< Máxima cantidad de usuarios registrados */
#define MAX_TRIES 3                    /*!< Máxima cantidad de usuarios intentos de autenticación */

/**
 * Estructura que representa la cantidad de intentos por nombre de usuario 
 */
typedef struct UserTries {
    char username[CREDENTIALS_SIZE];
    uint32_t count;
} UserTries;

/**
 * Estructura que representa las base de datos
 * de los intentos realizados por cada usuario
 */
typedef struct TriesDB {
    UserTries triesArray[CREDENTIAL_LIMIT_AMOUNT];    /*!< DB de intentos */
    size_t dbSize;                               /*!< Tamaño de la base de datos */
} TriesDB;

TriesDB triesDB = {.dbSize = 0u};

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

int checkCredentials(const Credentials* inputCredential, const Credentials* csvCredentials, const size_t credAmount)
{
    uint32_t i;
    for (i=0u; i<credAmount; i++) {
        if (credentialsEqual(inputCredential, csvCredentials+i)) {
            return true;
        }
    }
    return false;
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
    for (i = 0; (size_t)i<triesDB.dbSize; i++) {
        if (strncmp(triesDB.triesArray[i].username, username, CREDENTIALS_SIZE) == 0) {
            return i;
        }
    }
    return -1;
}

/**
 * Si no existe una entrada en la base de datos de intentos para un usuario
 * en particular la crea y pone su contador a 1. Si ya existe aumenta el contador.
 * @param username nombre de usuario
 */
void updateUserTry(const char* username)
{   
    /* Buscamos en el array de intentos si existe el usuario */
    int userIndex = findUser(username);
    if (userIndex != -1) {
        /* Si existe, aumentamos su contador */
        triesDB.triesArray[userIndex].count++;
    } else {
        /* ...sino, agregamos un nuevo usuario */
        userIndex = (int)triesDB.dbSize; 
        strncpy(triesDB.triesArray[userIndex].username, username, CREDENTIALS_SIZE);
        triesDB.triesArray[userIndex].count = 1u;
        triesDB.dbSize++;
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
        return triesDB.triesArray[userIndex].count >= MAX_TRIES;
    }
    return false;
}

#endif