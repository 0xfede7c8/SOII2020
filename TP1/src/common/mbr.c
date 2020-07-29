#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "mbr.h"

bool isMBR(const MBR *mbr)
{
    return mbr->bootSignature[0] == 0x55u &&
           mbr->bootSignature[1] == 0xAAu;
}

bool getMBR(const char *filePath, MBR *mbr) {
    FILE *fp = fopen(filePath, "rb");
    bool result = fp != NULL; 
    if (result) {
        const size_t n = fread(mbr, 1, sizeof(MBR), fp);
        result = n == sizeof(MBR) && isMBR(mbr);
        fclose(fp);
    }
    return result;
}

void printMBRInfo(const char *filePath)
{
    MBR mbr;
    if (getMBR(filePath, &mbr)) {
        printf("MBR Info: \n");
        uint32_t i;
        /* Iteramos por cada partición */
        for (i = 0u; i<PARTITIONS_AMOUNT; i++) {
            /* Si el tamaño de una partición es mayor a 0 significa que existe */
            if (mbr.partTable[i].size > 0) {
                printf("\tPartición %u:\n", i);
                printf("\t\tTamaño: %u\n", mbr.partTable[i].size);
                printf("\t\tTipo: %02x\n", mbr.partTable[i].partType);
                printf("\t\tSector de inicio: %u\n", mbr.partTable[i].startSector);
                printf("\t\tBooteable: %s\n", mbr.partTable[i].bootIndicator == BOOTABLE_FLAG ? "true" : "false");
            }
        }
    } else {
        printf("[-] Can't get MBR from file\n");
    }
}