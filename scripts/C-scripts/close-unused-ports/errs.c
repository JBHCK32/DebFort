#include "close-ports.h"
#include <stdio.h>


static void MESSAGE_ERR_SOCKET(const unsigned int PORT) {
    printf("\n\n------------------------------------------------------------------------------------------------------------");
    printf("\n>> Español: ");
    fprintf(stderr, "\n\e[1;31m>>[ERROR]: Ocurrio un error al querer abrir un socket para cerrar el puerto [%u]...\n", PORT);
    printf(">> Esto Pudo haber ocurrido por varias cosas aunque te digo que no es normal que esto\n");
    printf(">> suceda en linux's modernos:\n");
    printf(">> - Memoria insuficiente para crear un nuevo socket...\n");
    printf(">> - Limite de descriptores de archivo abiertos....\n");
    printf(">> - El dominio 'AF_INET' no esta soportado...\n");
    printf(">> Aqui mi recomendación es no entrar en panico, si necesitas realmente esto entonces\n");
    printf(">> elimina procesos, libera memoria si la estas usando al maximo, trata de que hayan la menor\n");
    printf(">> cantidad de procesos de fondo he intente otra vez.\n");

    printf("\n>> English: "); 
    fprintf(stderr, "\n\e[1;31m>>[ERROR]: An error occurred when wanting to open a socket to close port [%u]...\n", PORT); 
    printf(">> This could have happened for several reasons, although I tell you that it is not normal for this\n"); 
    printf(">> happens in modern Linux:\n"); 
    printf(">> - Insufficient memory to create a new socket...\n"); 
    printf(">> - Limit of open file descriptors....\n"); 
    printf(">> - The domain 'AF_INET' is not supported...\n"); 
    printf(">> Here my recommendation is not to panic, if you really need this then\n"); 
    printf(">> delete processes, free up memory if you're using it to the fullest, try to make them have the least\n"); 
    printf(">> number of background processes I've tried again.\n");
    printf("\n\n------------------------------------------------------------------------------------------------------------");
}

static void MESSAGE_ERR_SOCKET_BIND(const unsigned int PORT) {
    printf("\n\n---------------------------------------------------------------------------------------------");
    printf("\n>> Español: ");
    fprintf(stderr, "\n\e[1;31m>>[ERROR]: Hubo un error al querer cerrar el puerto: %u...\e[0m\n", PORT);
    printf(">> Esto pudo haber ocurrido por dos cosas:\n");
    printf(">> - Que el puerto que intentaste abrir esta ocupado por otro proceso(Probable).\n");
    printf(">> - Que el puerto que escribiste esta definitivamente cerrado.\n");
    printf(">> Al final si el puerto esta en la lista de puertos abiertos que te muestra el\n");
    printf(">> programa entonces es 100 que el puerto que intentaste cerrar esta ocupado\n");
    printf(">> por otro servicio, sino el puerto ya esta cerrado...\n");

    printf("\n>> English: ");
    fprintf(stderr, "\n\e[1;31m>>[ERROR]: There was an error when wanting to close the port: %u... \e[0m\n", PORT); 
    printf(">> This could have happened for two reasons:\n");
    printf(">> - That the port you tried to open is occupied by another process(Likely).\n"); 
    printf(">> - That the port you typed is definitely closed.\n"); 
    printf(">> At the end if the port is in the list of open ports shown by the \n"); 
    printf(">> program then is 100 that the port you tried to close is busy\n"); 
    printf(">> for another service, otherwise the port is already closed...\n");
    printf("-------------------------------------------------------------------------------------------------\n\n");
}

static void MESSAGE_ERR_DIRECT_PORT(const char *SERVICE, const unsigned int PORT) {
    printf("\n>> Español: ");
    fprintf(stderr, "\n>>[ERROR]: El puerto [%u] del servicio %s no existe o ya esta cerrado...", PORT, SERVICE);
    printf("\n>> English: ");
    fprintf(stderr, "\n>>[ERROR]: The port [%u] of the %s service does not exist or is already closed...\n", PORT, SERVICE);
}

void Verification_ERR_PORT(const uint8_t CODE_ERR, const uint16_t PORT, void* ADD_MESSAGE) {

    switch (CODE_ERR) {

        case ERR_SOCKET_BIND:
            MESSAGE_ERR_SOCKET_BIND((const unsigned int)PORT);
            return;

        case ERR_SOCKET:
            MESSAGE_ERR_SOCKET((const unsigned int)PORT);
            return;

        case ERR_DIRECT_PORT:
            MESSAGE_ERR_DIRECT_PORT((const char*)ADD_MESSAGE, (const unsigned int)PORT);
            return;

        default:
            printf(" ");
            return;
    }
}
