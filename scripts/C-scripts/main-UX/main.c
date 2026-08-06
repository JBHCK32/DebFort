#include "lib-gui.h"

// Important: (---)
// Context:   (+++)


// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Estas librerias son simplemente para poder usar signal sin ningun problema.
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <libgen.h>
#include <limits.h>
#include <signal.h> 

int main() {

    const uint8_t ROOT_ACCESS = 0;

    // ------------------------------------------------------------
    // 1. VERIFICACIÓN DE PERMISOS DE ROOT
    // ------------------------------------------------------------
    if (geteuid() != ROOT_ACCESS) {
        fprintf(stderr, "\n\x1b[1;31m[ERROR] Este programa debe ejecutarse con 'sudo' o como usuario root.\x1b[0m\n");
        fprintf(stderr, "\x1b[1;33m[INFO] Por favor, ejecuta: sudo ./build/debfort\x1b[0m\n\n");
        return (1);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++
    // 1. Cambiar al directorio raíz del proyecto.
    // +++++++++++++++++++++++++++++++++++++++++++++++
    char exe_path[PATH_MAX];

    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);

    if (len == -1) {
        perror("readlink failed");
        return 1;
    }

    exe_path[len] = '\0';

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // 2. Luego obtenemos el nombre del path en el que se esta ejecutando del programa,
    // que en este caso seria el nuevo directorio raiz. 
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    char *dir = dirname(exe_path); //-> dir = .../build
    char new_root_path[PATH_MAX];

    snprintf(root_path, sizeof(root_path), "%s/..", dir);

    if (chdir(new_root_path) != SUCCESS) {
        Verification_ERR_MENU(ERR_CHDIR);
        return (ERR_CHDIR);
    } 

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Para que no ocurra ninguna interrupción en medio del programa y
    // se detenga en un proceso importante, usamos la signal para
    // atrapar la señal del Ctrl+C que se le nombra como SIGINT y
    // le mandamos una función mostrandole un simple mensaje de error.
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    signal(SIGINT, L_MESSAGE_ERR);

    char Option[MAX_RANGE_OP];

    while(true) {

        sleep(GENERIC_SLEEP);
        clear();
        Menu_Init();

        const uint8_t Code_Result_Input = Processing_InputChar(Option, MAX_RANGE_OP);

        if (Code_Result_Input == ERR_SIGINIT) {
            Message_ERR();
            continue;
        }

        else if (Code_Result_Input != SUCCESS) {
            Verification_ERR_MENU(Code_Result_Input);
            continue;
        }

        if (Option[0] == '1') {
            Menu_Services();
            continue;
        }

        else if (Option[0] == '2') {
            Menu_Ports();
            continue;
        }

        else if (Option[0] == '3') {
            Menu_Firewall();
            continue;
        }

        else if (Option[0] == '4') {
            Menu_Privileges();
            continue;
        }

        else if (Option[0] == '5') {
            Menu_SSH();
            continue;
        }

        else if (Option[0] == '6') {
            return (SUCCESS);
        }

        else {
            printf("\n\x1b[31m>> Opción Incorrecta...\x1b[0m\n");
            continue;
        }
    }

    return (SUCCESS);
}
