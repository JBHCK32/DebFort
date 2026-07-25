#include "close-ports.h"

// Important: (---)
// Context:   (+++)

// ---------------------------------------------------------------
// Esta función crea un socket basico para interactuar con un 
// puerto que recibe y cerrarlo y cerrar el socket, cerrando
// asi el puerto de manera sencilla. 
// ---------------------------------------------------------------
static const uint8_t Close_Port(uint16_t PORT_USER) {

    int server_fd;
    struct sockaddr_in address;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Creamos el socket con el que vamos a interactuar.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) != SUCCESS) {
        return (ERR_SOCKET);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++
    // Configuramos nuestro socket en donde damos:
    // - Dirección del socket.
    // - Puerto al que se va a conectar.
    // +++++++++++++++++++++++++++++++++++++++++++++++
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_USER); 

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Enlazamos nuestro socket con el puerto del usuario.
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++
    const int8_t Code_Result_Bind = bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    if (Code_Result_Bind != SUCCESS) {
        close(server_fd);
        return (ERR_SOCKET_BIND);
    }

    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Después de enlazar nuestro socket con el puerto que desea cerrar el usuario.
    // Lo cerramos de manera controlada.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    shutdown(server_fd, SHUT_RDWR);
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Y por ultimo cerramos el socket para liberar de manera definitiva el puerto.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    close(server_fd);

    return (SUCCESS);
}

const uint8_t CLOSE_PORTS(char *PORT_USER) {


}
