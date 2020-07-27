#ifndef MD5_H

#define MD5_H

#include <stdbool.h>
#include <stdio.h>

#include <openssl/md5.h>

bool calculateMD5(const char *filename, unsigned char *digest, size_t *fileSize)
{
    FILE *inFile = fopen (filename, "rb");
    bool result = inFile != NULL;
    if (result) {
        MD5_CTX mdContext;
        size_t bytes;
        unsigned char data[1024];

        MD5_Init (&mdContext);
        while ((bytes = fread (data, 1, 1024, inFile)) != 0) {
            MD5_Update(&mdContext, data, bytes);
            *fileSize += bytes;
        }
        MD5_Final(digest,&mdContext);
        fclose (inFile);
    }
    return result;
}

#endif