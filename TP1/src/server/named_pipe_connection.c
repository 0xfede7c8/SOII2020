#include "named_pipe_connection.h"

int getFIFOs(int* readfd, int* writefd)
{
    *readfd = getFIFORead(SERVER_READ_FIFO_NAME);
    *writefd = getFIFOWrite(SERVER_WRITE_FIFO_NAME);
    return (*readfd > 0) && (*writefd > 0);
}