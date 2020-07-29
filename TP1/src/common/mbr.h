#ifndef MBR_H

#define MBR_H

#include <stdint.h>
#include <stdbool.h>

#define SECTOR_SIZE 512u

#define BOOTABLE_FLAG 0x80u
#define NOT_BOOTABLE_FLAG 0x00u
#define FAT32_TYPE_FLAG 0x0Bu
#define PARTITIONS_AMOUNT 4u

#define SECTOR_MASK 0x3Fu
#define CYLINDER_MASK 0x3FF

typedef struct  __attribute__((__packed__)) Partition {
    uint8_t bootIndicator;
    uint8_t startAddrHead;
    uint16_t startAddrCylSec;
    uint8_t partType;
    uint8_t endAddrHead;
    uint16_t endAddrCylSec;
    uint32_t startSector;
    uint32_t size;
} Partition;

typedef struct  __attribute__((__packed__)) MBR {
    uint8_t code[440];
    uint32_t diskSig;
    uint16_t reserved;
    Partition partTable[PARTITIONS_AMOUNT];
    uint8_t bootSignature[2];
} MBR;

bool isMBR(const MBR *mbr);

bool getMBR(const char *filePath, MBR *mbr);

void printMBRInfo(const char *filePath);

#endif