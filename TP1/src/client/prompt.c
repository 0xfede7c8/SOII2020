#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "prompt.h"

char* getCurrentTime();

const char* username; 

void printASCIIArt()
{
	printf("\n-------------------------------------------------------\n");
	printf(" _   ,--()\n");
	printf("( )-'-.------|> Creador online de USBs booteables \n");
	printf(" \"     `--[]\n");
	printf("-------------------------------------------------------\n\n");
    printf("[-] Para conocer los comandos disponibles, ecribir \"help\"\n\n");
}

void setPromptUsername(const char* newUsername) 
{
	username = newUsername;
}

void printPrompt() {
    char* currentTime = getCurrentTime(); 
    printf("[%.19s %s] ", currentTime, username);
}