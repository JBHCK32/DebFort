#include "lib-gui.h"

// --------------------------------------------------------------------------------------------
// Esta es una función general que obtiene el input del usuario y lo guarda en una variable.
//
// Solamente obtiene el input del usuario y lo guarda con un limie impuesto por el cliente.
// --------------------------------------------------------------------------------------------
uint8_t Processing_InputChar(char *CONTAINER, size_t MAX_RANGE) {

    if (fgets(CONTAINER, MAX_RANGE + 2, stdin) == NULL) {
        printf("\n>> Porfavor no intente detener el programa...\n");
        return (ERR_SIGINIT);
    }

    uint8_t Size_Container = (uint8_t)str_len(CONTAINER);

    if (Size_Container >= MAX_RANGE && CONTAINER[Size_Container - 1] != '\n') {
        int16_t c;
        while ((c = getchar()) != '\n' && c != EOF);
        clearerr(stdin);

        return (ERR_MEMORY_BUFFER);
    }

    clearerr(stdin);

    CONTAINER[Size_Container - 1] = '\0';

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Con esto limpiamos toda la basura que haya escrito el usuario.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    if (freopen("/dev/tty", "r", stdin) == NULL) { 
        return (ERR_OPEN_TTY);
    }

    return (SUCCESS);
}





