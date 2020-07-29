#ifndef MD5_H

#define MD5_H

#include <stdbool.h>
#include <stdio.h>

#include <openssl/md5.h>

#define BLOCK_SIZE 1024

/**
 * Calcula el hash MD5 a partir de un archivo
 *
 * @param filename archivo a procesar
 * @param digest donde se almacenará el resultado
 * @param amountProcessed cantidad de datos procesadas en bytes, si limit > 0, amountProcessed == limit, sino amountProcessed == size
 * @param limit cantidad de bytes del archivo a calcular. limit = 0 para leer todo el archivo
 * @return bool con resultado de la operación
 */
bool calculateMD5(const char *filename, unsigned char *digest, size_t *amountProcessed, size_t limit)
{
    FILE *inFile = fopen (filename, "rb");
    bool result = inFile != NULL;
    if (result) {
        MD5_CTX mdContext;
        size_t bytes;
        unsigned char data[BLOCK_SIZE];

        MD5_Init (&mdContext);
        (*amountProcessed) = 0u;
        if (limit == 0u) {
            while ((bytes = fread (data, 1, BLOCK_SIZE, inFile)) != 0) {
                MD5_Update(&mdContext, data, bytes);
                *amountProcessed += bytes;
            }
        } else if (limit > 0u){
            /* En este caso solo calculamos hasta el limite */
            while ((bytes = fread (data, 1, BLOCK_SIZE, inFile)) != 0 && (*amountProcessed < limit)) {
                MD5_Update(&mdContext, data, bytes);
                *amountProcessed += bytes;
            }
        } else {
            result = false;
        }
        if (result) {
            MD5_Final(digest,&mdContext);
            fclose (inFile);
        }
    }
    return result;
}

/**
 * Imprime el digesto MD5 a partir de su representación binaria
 *
 * @param digest arreglo a unsigned char con el digesto a imprimir
 */
void printMD5Digest(const unsigned char *digest)
{   
    size_t j;
    for (j=0u; j<MD5_DIGEST_LENGTH; j++)
    {
        printf("%02x", digest[j]);
    }
    printf("\n");
}

#endif