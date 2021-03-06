/**
 * @file
 * @brief Definiciones de mensajes del protocolo del cliente y servidor
 */

#ifndef COMMANDS_H
#define COMMANDS_H

/**
 * Enum que contiene los mensajes de control posibles entre el cliente y servidor
 */
typedef enum Message {
    CLIENT_EXIT = 0u,        /*!< Finalización de la conexión por el cliente */
    AUTHENTICATE_REQUEST,    /*!< Pedido del cliente para autenticarse */ 
    AUTHENTICATE_PROCEED,    /*!< Pedido del cliente para autenticarse concedido */ 
    AUTHENTICATE_BLOCKED,    /*!< Autenticación bloqueada por el servidor */ 
    AUTHENTICATE_FAILED,     /*!< Autenticación fallida */ 
    AUTHENTICATE_PASSED,     /*!< Autenticación exitosa */ 
    USER_LIST,               /*!< Pedido de lista de usuario */ 
    USER_LIST_FINISH,        /*!< Mensaje de terminación de envío de lista de usuarios */ 
    USER_PASSWORD,           /*!< Pedido de cambio de contraseña */ 
    FILE_LIST,               /*!< Pedido de listado de archivos */ 
    FILE_DOWN,               /*!< Pedido de descarga de archivo */
    FILE_DOWN_AUTHORIZED,    /*!< Autorización para descargar */
    FILE_DOWN_REJECTED,      /*!< Descarga rechazada */
    GET_FILE_SERVER_PORT,    /*!< Pedido de información del puerto del file server */
    MESSAGE_SUCCESS,         /*!< Envío o recepción de mensaje exitoso */ 
    MESSAGE_FAILED,          /*!< Envío o recepción de mensaje fallida */ 
} Message;

#endif