/**
 * @file
 * @brief Parseo de contraseñas en formato CSV
 */
#ifndef CSV_H

#define CSV_H

#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "message_transmission.h"

/**
 * Separa cada linea por "," y devuelve el item seleccionado
 *
 * @param line puntero a char con la linea a parsear
 * @param num entero con el elemento a seleccionar
 * @return puntero al token elegido
 */
const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

/**
 * Llena un arreglo de Credentials con las credenciales parseadas del CSV
 *
 * @param credentials Arreglo de credenciales a completar
 * @param credentialsSize Tamaño del arreglo alocado por el usuario
 * @param csvPath Path al archivo de credenciales en formato CSV
 * @return cantidad de credenciales parseadas
 */
size_t parseUserDataCSV(Credentials* credentials, const size_t credentialsSize, const char* csvPath)
{
    size_t credentialIndex = 0u;   
    FILE* stream = fopen(csvPath, "r");

    if (stream != NULL) {
        const size_t lineSize = CREDENTIALS_SIZE*2 + 2;    // Cada linea puede tener un usuario y un password
        char line[lineSize];
        while (fgets(line, (int)lineSize-1, stream) && (credentialIndex < credentialsSize)) {
            char* tmp = strdup(line);
            strncpy((credentials+credentialIndex)->username, getfield(tmp, 1), CREDENTIALS_SIZE);
            free(tmp);

            tmp = strdup(line);
            strncpy((credentials+credentialIndex)->password, getfield(tmp, 2), CREDENTIALS_SIZE);
            free(tmp);

            credentialIndex++;
        }
    }
    return credentialIndex;
} 

#endif