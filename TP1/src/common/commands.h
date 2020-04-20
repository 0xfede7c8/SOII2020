#ifndef COMMANDS_H
#define COMMANDS_H

enum Message {
	EXIT = 0u,
	OK,
	AUTHENTICATE_REQUEST,    // Pedido del cliente para autenticarse
	AUTHENTICATE_PROCEED,    // Pedido del cliente para autenticarse concedido
	AUTHENTICATE_BLOCKED,    // Autenticación bloqueada por el servidor
	AUTHENTICATE_FAILED,     // Autenticación fallida
	AUTHENTICATE_PASSED,     // Autenticación exitosa
	USER_LIST,
	USER_PASSWORD,
	FILE_LIST,
	FILE_DOWN,
	MESSAGE_SUCCESS,         // Envío o recepción de mensaje exitoso
	MESSAGE_FAILED,          // Envío o recepción de mensaje fallida
};

#endif