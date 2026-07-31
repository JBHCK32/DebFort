#include "close-ports.h"

// Important: (---)
// Context:   (+++)

#define LIST_PORTS 21
#define MAX_RANGE_LIST_PORTS (LIST_PORTS + 1)

#define LIST_SERVICES 21
#define MAX_RANGE_LIST_SERVICES (LIST_SERVICES + 1)

#define NAME_SERVICES 20
#define MAX_RANGE_NAME_SERVICES (NAME_SERVICES + 1)

// --------------------------------------------------------------------------------
// Esta es la lista de los puertos de cada uno de los servicios
// que desactiva el archivo script/shell-scripts/cleaning-services.sh.
// 
// La idea es que después del proceso de desactivación ahora ocurra el
// proceso de desactivación de puertos que utilizaban de manera estandarizada
// cada uno de los servicios, como es algo estandarizado y que no va a mutar
// entonces decidi que fuera algo simple y facil de manera como listas rigidas.
// --------------------------------------------------------------------------------
static const uint16_t LIST_CLOSE_PORTS[MAX_RANGE_LIST_PORTS] = {
    5353, // Avahi
    631, // CUPS
    111, // rpcbind
    2049, // NFS
    20048, // NFS (mountd) 
    662, // NFS (statd) 
    4045, // NFS (lockd)
    139, // Samba (smbd)
    445, // Samba (smbd)
    137, // Samba (nmbd)
    138, // Sambba (nmbd)
    23, // Telnet
    21, // FTP
    20, // FTP
    69, // TFTP
    514, // RSH
    513, // Rlogin
    123, // NTP / Chrony
    25, // Postfix
    587, // Postfix
    465 // Postfic
};


// --------------------------------------------------------------------------------------------------
// En general esta lista de nombres de los servicios es unicamente para enseñarlo en el mensaje
// de error y se repite nombres para que este coordinada con la lista de puertos.
//
// Esto es solo algo estetico y por eso lo hice tan rigido.
// --------------------------------------------------------------------------------------------------
static const char LIST_NAME_SERVICES[MAX_RANGE_LIST_SERVICES][MAX_RANGE_NAME_SERVICES] = {
    "avahi-daemon.service",
    "cups.service",
    "rpcbind.service",
    "nfs-server.service",
    "nfs-server.service",
    "nfs-server.service",
    "nfs-server.service",
    "smbd.service",
    "smbd.service",
    "nmbd.service",
    "nmbd.service",
    "telnetd.service",
    "vsftpd.service",
    "vsftpd.service",
    "tftpd-hpa.service",
    "rsh.service",
    "rlogin.service",
    "ntp.service",
    "postfix.service",
    "postfix.service",
    "postfix.service"
};

// ----------------------------------------------------------------------------------
// Esta es una función que cierra uno por uno los puertos de los servicios que 
// se desactivan tambien de manera automatica, esto completa la parte de puertos
// y servicios inecesarios eliminados de formado automatica.
// ----------------------------------------------------------------------------------
static bool Automatic_Close_Ports() {

    for (uint8_t i = 0; i < MAX_RANGE_LIST_PORTS; i++) {
        
        const uint16_t Port = LIST_CLOSE_PORTS[i];

        const uint8_t Code_Result_Close = CLOSE_DIRECT_PORT(Port);
        
        if (Code_Result_Close == ERR_SOCKET) {
            Verification_ERR_PORT(Code_Result_Close, Port, NULL);
            return (false);
        }

        else if (Code_Result_Close != SUCCESS) {
            Verification_ERR_PORT(ERR_DIRECT_PORT, Port, (void*)LIST_NAME_SERVICES[i]);
            continue;
        }
        
    }

    return (true);
}


// -------------------------------------------------------------------
// Esta es la función publica con la que interactuara el menu
// para ejecutar el proceso de cerrar puertos de manera automatica.
// -------------------------------------------------------------------
bool INIT_AUTOMATIC_CLOSE_PORTS() {
    return (Automatic_Close_Ports());
}



