#include "named_pipe_connection.h"

bool getFIFOs(int* readfd, int* writefd)
{
    /* El orden con el cual se abren las FIFOS
     * es muy importante. De ser cambiado produciría el 
     * bloqueo del sistema */
    *writefd = getFIFOWrite(SERVER_WRITE_FIFO_NAME);
    *readfd = getFIFORead(SERVER_READ_FIFO_NAME);
    return (*readfd > 0) && (*writefd > 0);
}