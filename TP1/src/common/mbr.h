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

/**
 * Estructura que representa una particion de MBR 
 */
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

/**
 * Estructura que representa el header de MBR 
 */
typedef struct  __attribute__((__packed__)) MBR {
    uint8_t code[440];
    uint32_t diskSig;
    uint16_t reserved;
    Partition partTable[PARTITIONS_AMOUNT];
    uint8_t bootSignature[2];
} MBR;

/**
 * Chequea si la estructura MBR mbr tiene la signature correcta de MBR
 *
 * @param mbr puntero a estructura MBR
 * @return bool con resultado de la operación
 */
bool isMBR(const MBR *mbr);

/**
 * A partir de un archivo, obtiene su header MBR y lo almacena en una estructura
 *
 * @param filePath nombre del archivo
 * @param mbr puntero a estructura MBR donde se almacena el resultado
 * @return bool con resultado de la operación
 */
bool getMBR(const char *filePath, MBR *mbr);

/**
 * Imprime informacion sobre el header MBR
 *
 * @param filePath nombre del archivo
 */
void printMBRInfo(const char *filePath);

#endif