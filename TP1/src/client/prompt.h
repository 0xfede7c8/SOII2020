/**
 * @file
 * @brief Funciones relativas al prompt
 */
#ifndef PROMPT_H

#define PROMPT_H

void printASCIIArt();

/**
 * Setea el nombre de usuario que mostrará el prompt
 *
 * @param username puntero a char con el nombre de usuario
 */
void setPromptUsername(char* username);

/**
 * Tiene la misma interfaz que printf, pero imprime al principio un prompt para el usuario
 *
 * @param *fmt igual que printf
 */
void printPrompt();

#endif