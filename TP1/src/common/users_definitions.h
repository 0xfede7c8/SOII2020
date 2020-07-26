#ifndef USERS_DEFINITIONS_H

#define USERS_DEFINITIONS_H

#include <stdint.h>

#define CREDENTIALS_SIZE 30
#define MAX_STRINGS_SIZE 30
#define CREDENTIAL_LIMIT_AMOUNT 10u    /*!< Máxima cantidad de usuarios registrados */

/**
 * Estructura que representa las credenciales del usuario 
 */
typedef struct Credentials {
    char username[CREDENTIALS_SIZE];    /*!< nombre de usuario */
    char password[CREDENTIALS_SIZE];    /*!< contraseña */
} Credentials;

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
    int lastUserIndex;                           /*!< Indice del último usuario en logearse */
} UserDB;

#endif