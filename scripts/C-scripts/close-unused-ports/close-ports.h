#ifndef _CLOSE_UNUSED_PORTS_H_
#define _CLOSE_UNUSED_PORTS_H_

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SUCCESS 0x00

#define ERR_SOCKET_BIND 0x01
#define ERR_SOCKET 0x02

#define ERR_DIRECT_PORT 0x03

// ---------------------------------------------------------------------
// close-ports,c: Este archivo contiene funciones
// especificas a la hora de cerrar puertos UPD o TCP sin problemas.
// Existe la opción de cerrarlo de manera directa solo pasando el 
// número del puerto.
// ---------------------------------------------------------------------

uint8_t CLOSE_DIRECT_PORT(const uint16_t PORT_USER);

bool CLOSE_PORT_TO_MENU(const uint16_t PORT_USER);


// ------------------------------------------------------------------
// automatic-list-ports.c : 
//
// En este archivo solo esta presente una lista de los puertos 
// de los servicios que se desactivan de manera automatica, estan 
// enlazados los servicios que se desactivan de manera automatica
// en script/shell-scripts/cleaningn-services.sh, la idea
// es que después de la ejecución del script y la desactivación
// de los servicios de cierren de manera automatica los puertos
// estandar por los cuales se comunicaban estos servicios.
// ------------------------------------------------------------------
bool INIT_AUTOMATIC_CLOSE_PORTS();

// ----------------------------------------------------------------------------------
// Esta es una función que se repite mucho en cada libreria y es para mostrar
// los errores especificos he internos de lo que sucede en los procesos
// internos de la libreria.
// ----------------------------------------------------------------------------------
void Verification_ERR_PORT(const uint8_t CODE_ERR, const uint16_t PORT, void* ADD_MESSAGE);

#endif
