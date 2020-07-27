#ifndef FILES_DEFINITIONS_H

#define FILES_DEFINITIONS_H

#include <openssl/md5.h>

#define MAX_FILES_AMOUNT 100u
#define MAX_FILENAME_LENGTH 256u

#define FILES_DIR_PATH "bin/files"

/**
 * Estructura que representa información sobre
 * un archivo
 */
typedef struct FileInfo {
    char fileName[MAX_FILENAME_LENGTH];
    unsigned char md5Sum[MD5_DIGEST_LENGTH];
    size_t size;
} FileInfo;

/**
 * Estructura que representa las base de datos
 * de archivos disponibles
 */
typedef struct FileInfoDB {
    FileInfo userInfo[MAX_FILES_AMOUNT];    /*!< Arreglo de información de archivos */
    size_t dbSize;                          /*!< Tamaño de la base de datos */
} FileInfoDB;

#endif