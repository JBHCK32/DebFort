#include "close-ports.h"

// Important: (---)
// Context:   (+++)

// -------------------------------------------------------------------
// Esta función crea un socket basico para interactuar con un 
// puerto que recibe y cerrarlo y cerrar el socket, cerrando
// asi el puerto de manera sencilla.
//
// Es modular y aplica para cualquier protocolo
// ya que al final solo vamos a cerrar el puerto, no a comunicarnos
// a travez de él.
// -------------------------------------------------------------------
static const uint8_t Close_Port(const uint16_t PORT_USER) {

    int server_fd;
    struct sockaddr_in address;
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Creamos el socket con el que vamos a interactuar.
    //
    // Al final esto es necesario para poder
    // interactuar con el puerto que vamos a cerrar.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < SUCCESS) {
        return (ERR_SOCKET);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++
    // Configuramos nuestro socket en donde damos:
    //
    // - Dirección del socket.
    // - Puerto al que se va a conectar.
    // +++++++++++++++++++++++++++++++++++++++++++++++
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT_USER); 

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Enlazamos nuestro socket con el puerto del usuario
    //
    // y esto nos permite cerrar el socket y a lavez cerrar
    // el puerto.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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



// ---------------------------------------------------------------
// Esta es una función especifica para poder cerrar puertos de 
// manera directa sin tener que hacer un casteo de por medio.
// ---------------------------------------------------------------
const uint8_t CLOSE_DIRECT_PORT(const uint16_t PORT_USER) {

    const uint8_t Code_Result_Close = Close_Port(PORT_USER);

    if (Code_Result_Close != SUCCESS) {
        return (Code_Result_Close);
    }

    return (SUCCESS);
}

// ----------------------------------------------------------------------------------------------------
// Esta es una función con una gestion de errores que simplifica la interacción con el cliente.
//
// Si llegara a ocurrir un error en especifico todo se detiene y se le dice al cliente que detenga
// todo con el retorno false, sino simplemente muestar el error que ocurrio y se le dice al usuario
// que error ocurrio pero se retorna que la operación fue exitosa ya que el unico error que puede
// ocurrir es que el puerto este ocupado o cerrado y se le comunica al usuario.
// ----------------------------------------------------------------------------------------------------
const bool CLOSE_PORT_TO_MENU(const uint16_t PORT_USER) {

    const uint8_t Code_Result_Close = Close_Port(PORT_USER);

    if (Code_Result_Close == ERR_SOCKET) {
        Verification_ERR_PORT(Code_Result_Close, PORT_USER);
        return (false);
    }

    else if (Code_Result_Close != SUCCESS) {
        Verification_ERR_PORT(Code_Result_Close, PORT_USER);
    }

    return (true);
}
