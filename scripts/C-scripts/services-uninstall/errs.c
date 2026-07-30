#include "services-uninstall.h"
#include <stdio.h>


static void MESSAGE_ERR_SERVICE_NOT_EQ() {
    printf("\n\n----------------------------------------------------------------------------------------------------------------");
    printf("\n>> Español: ");
    fprintf(stderr, "\n\x1b[1;31m>>[ERROR]: El servicio que escribiste no existe o ya esta desactivado o enmascarado...\x1b[0m\n");
    printf("\n>> English: "); 
    fprintf(stderr, "\n\x1b[1;31m>>[ERROR]: The service you typed doesn't exist or is already disabled or masked...\x1b[0m\n");
    printf("----------------------------------------------------------------------------------------------------------------\n\n");
}

static void MESSAGE_ERR_POPEN() {
    printf("\n\n----------------------------------------------------------------------------------------------------------------------------------------------");
    printf("\n>> Español: ");
    fprintf(stderr, "\n\x1b[1;31m>>[ERROR]: Ocurrio un error critico al quere usar popen...\x1b[0m\n");
    printf(">> Mira no te asustes, este es un error raro pero tiene sus causas probables:\n");
    printf(">> - Puede ser que tu PATH este mal configurado o modificado y eso permitio no poder ejecutar systemctl.(Poco Probable)\n");
    printf(">> - Puede ser que tu sistema ya haya llegado al limite de descriptores de archivos, osea tienes muchos archivos abiertos.(Probable)\n");
    printf(">> Estos errores son faciles de solucionar pero ocurre en casos extremos, solo verifica que alguno de esos sea tu caso y listo.\n");

    printf("\n>> English: "); 
    fprintf(stderr, "\n\x1b[1; 31m>>[ERROR]: A critical error occurred when I wanted to use popen...\x1b[0m\n"); 
    printf(">> Look don't panic, this is a rare mistake but it has its probable causes:\n"); 
    printf(">> - It could be that your PATH is misconfigured or modified and that allowed you to not be able to run systemctl.( Unlikely)\n"); 
    printf(">> - It may be that your system has already reached the limit of file descriptors, so you have a lot of files open.( Probable)\n"); 
    printf(">> These errors are easy to fix but it happens in extreme cases, just check that one of those is your case and that's it.\n");
    printf("----------------------------------------------------------------------------------------------------------------------------------------------\n\n");
}

void Verification_ERR_SERVICES(const uint8_t CODE_ERR) {

    switch(CODE_ERR) {
        case ERR_SERVICE_NOT_EQ:
            MESSAGE_ERR_SERVICE_NOT_EQ();
            return (void);

        case ERR_POPEN:
            MESSAGE_ERR_POPEN();
            return (void);

        default:
            printf("Unknown Error: %u", (const unsigned int)CODE_ERR);
            return (void);
    }
}

