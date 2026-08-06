#include "lib-gui.h"

static void MESSAGE_MEMORY_BUFFER() {
    printf("\x1b[31m[ERROR]: Ocurrio un error que sobre paso la memoria e buffer de una variable..\x1b[0m\n");
}

void Verification_ERR_MENU(const uint8_t CODE_ERR) {
    switch(CODE_ERR) {
        case ERR_MEMORY_BUFFER:
            MESSAGE_MEMORY_BUFFER();
            return;
        default:
            printf(" ");
            return;
    }
}


