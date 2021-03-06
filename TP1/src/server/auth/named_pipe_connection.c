#include "named_pipe_connection.h"

bool getFIFOs(int* readfd, int* writefd)
{
    *readfd = getFIFORead(AUTH_SERVICE_READ_FIFO_NAME);
    *writefd = getFIFOWrite(AUTH_SERVICE_WRITE_FIFO_NAME);
    
    return (*readfd > 0) && (*writefd > 0);
}

bool createFIFOs()
{
    return createFIFO(AUTH_SERVICE_READ_FIFO_NAME) && 
           createFIFO(AUTH_SERVICE_WRITE_FIFO_NAME);
}

bool createFIFO(const char* fifoPath)
{
    struct stat stats;
    // Chequeamos si la FIFO existe
    if (stat(fifoPath, &stats) < 0 )
    {
        if (errno != ENOENT)
        {
            perror("[-] Stat a la FIFO");
            return false;
        }
    }
    else
    {
        // Tratamos de borrar la FIFO existente
        if (unlink(fifoPath) < 0)
        {
            // Puede ser que algun otro proceso la esté usando
            perror("[-] Borrar FIFO");
            return false;
        }
    }

    if (mkfifo(fifoPath, 0666) < 0)
    {
        perror("[-] Creación de FIFO" );
        return false;
    }
    return true;
}