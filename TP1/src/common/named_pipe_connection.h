/**
 * @file
 * @brief Control de los named pipes (FIFOS) requeridos
 */
#ifndef NAMED_PIPE_CONNECTION_H

#define NAMED_PIPE_CONNECTION_H

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#define SERVER_WRITE_FIFO_NAME "bin/server_write"
#define AUTH_SERVICE_WRITE_FIFO_NAME "bin/auth_write"

#define SERVER_READ_FIFO_NAME AUTH_SERVICE_WRITE_FIFO_NAME
#define AUTH_SERVICE_READ_FIFO_NAME SERVER_WRITE_FIFO_NAME

bool createFIFOs();

inline int getFIFOfd(const char* fifo_name, int flag)
{
    return open(fifo_name, flag);
}

inline int getFIFORead(const char* fifo_name)
{
    return getFIFOfd(fifo_name, O_RDONLY);
}   

inline int getFIFOWrite(const char* fifo_name)
{
    return getFIFOfd(fifo_name, O_WRONLY);
}

/**
 * Obtiene los file descriptor de escritura y lectura.
 * Cada parte debe implementar este método por separado.
 *
 * @param readfd puntero a entero que contendrá el fd de lectura
 * @param writefd puntero a entero que contendrá el fd de escritura
 * @return bool dependiendo el resultado
 */
bool getFIFOs(int* readfd, int* writefd);

bool createFIFO(const char* fifoPath);

#endif