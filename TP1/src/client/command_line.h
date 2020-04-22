/**
 * @file
 * @brief Parseo de linea de comandos y ruteo hacia las diferentes funciones
 */

#ifndef COMMAND_LINE_H

#define COMMAND_LINE_H

#include <stdbool.h>
#include <string.h>

#include "prompt.h"
#include "utils.h"

#define MAX_CMD_LEN 512u
#define MAX_CMD_ARGS 32u

void printCLIHelp()
{
    printf("Comandos posibles: \n");
    printf("\tuser ls: Lista los usuarios registrados\n");
    printf("\tuser passwd: Cambia la contraseña del usuario\n");
    printf("\tfile ls: Lista las imagenes disponibles\n");
    printf("\tfile down <imagen>: Descarga la imagen seleccionada y crea el USB booteable\n");
    printf("\texit: Sale del programa, terminando el servidor\n");
    printf("\thelp: Imprime esta ayuda por stdout\n");
}

/**
 * Separa un comando en argc y argv
 *
 * @param command el comando a parsear
 * @param argv arreglo de punteros a char donde se guardarán los argumentos
 * @param argc puntero a int donde se guardará la cantidad de argumentos
 */
void splitCommand(char* command, char** argv, int* argc)
{
	*argc = 0;
  	char* arg = strtok(command, " ,.-");
	do {
        argv[*argc] = arg;
        (*argc)++;
        arg = strtok(NULL, " ,.-");
	} while (arg != NULL);
}

/**
 * Pide el ingreso de un commando por stdin
 *
 * @return el comando alocado estaticamente
 */
char* getCommand()
{
    char* result = NULL;
	static char command[MAX_CMD_LEN];
	memset(command, '\0', MAX_CMD_LEN);
	if (fgets(command, MAX_CMD_LEN-1, stdin) != NULL) {
        command[strcspn(command, "\n")] = 0;    // Eliminamos el \n
        result = command;
    }
    return result;
}

/**
 * Parsea y ejecuta el comando ingresado
 *
 * @param 
 * @return el comando alocado estaticamente
 */
int runCommand(const int argc, char* argv[], const int fd)
{	
	bool result = false; 
	if (argc > 0) {
		// Familia de comandos: "user"
		if (strcmp(argv[0], "user") == 0) {    // Familia de comandos: "user" 
			result = strcmp(argv[1], "ls") == 0;
			if (result){ // user ls
				printf("user ls\n");
			}
			else {   // user passwd 
				result = strcmp(argv[1], "passwd") == 0;
				if (result) {
					printf("user passwd\n");
				}
			}
		} 
		else if (strcmp(argv[0], "file") == 0) {    // Familia de comandos: "file"
			result = strcmp(argv[1], "ls") == 0;    
			if (result) {   // file ls
				printf("file ls\n");
			}
			else {    // file down
				result = strcmp(argv[1], "down") == 0;
				if (result) {
					printf("file down\n");
				}
			}
		}
		// Parseamos exit
		else if (strcmp(argv[0], "exit") == 0) {
            result = true;
			safeExit(fd);
		}
        // Parseamos exit
        else if (strcmp(argv[0], "help") == 0) {
            result = true;
            printCLIHelp();
        }
		else {
			result = false;
		}
	}

	result = (argc == 0) ? true : result;    // El caso donde no le pasamos ningun comando

	return result;
}

#endif