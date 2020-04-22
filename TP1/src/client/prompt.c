#include <stdio.h>
#include <time.h>

#include "prompt.h"

char* username; 

void printASCIIArt()
{
	printf("\n-------------------------------------------------------\n");
	printf(" _   ,--()\n");
	printf("( )-'-.------|> Creador online de USBs booteables \n");
	printf(" \"     `--[]\n");
	printf("-------------------------------------------------------\n\n");
    printf("[-] Para conocer los comandos disponibles, ecribir \"help\"\n\n");
}

char* getCurrentTime()
{
  time_t rawtime;
  time(&rawtime);
  return asctime(localtime(&rawtime));
}

void setPromptUsername(char* newUsername) 
{
	username = newUsername;
}

void printPrompt() {
    printf("[%.19s %s] ",getCurrentTime(), username);
}