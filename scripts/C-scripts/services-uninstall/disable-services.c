#include "services-uninstall.h"

#define INIT_COMMAND 20
#define MAX_RANGE_INIT_COMMAND (INIT_COMMAND + 1)

#define FINALE_COMMAND 26
#define MAX_RANGE_FINALE_COMMAND (FINALE_COMMAND + 1)

#define MAX_RANGE_COMMAND (COMMAND + INIT_COMMAND + MAX_RANGE_SERVICE_NAME + FINALE_COMMAND + MAX_RANGE_SERVICE_NAME + 1)


// -------------------------------------------------------------------------------------------
// Para poder usar esta función el tamaño de el string service no puede superar el tamaño
// de 219 caracteres visibles. La idea es que esta función haga y procese la logica de la
// creación del comando y lo que de vuelve system al ejecutarlo, sin mentirte
// no creo que ocurran muchos errores ya que al final antes de procesar el servicio
// ser verifica previamente que el servicio exista, sino simplemente no se procesara
// nada y se le notificara al usuario que el servicio que escribio o no existe
// o simplemente no esta activo en su maquina.
// -------------------------------------------------------------------------------------------
static const uint8_t Disable_Service(char *Service) {

    char Init_Command[MAX_RANGE_INIT_COMMAND] = "sudo systemctl stop ";
    char Finale_Command[MAX_RANGE_FINALE_COMMAND] = " && sudo systemctl disable ";

    char Command[MAX_RANGE_COMMAND];

    str_cat(Command, Init_Command);
    str_cat(Command, Service);
    str_cat(Command, Finale_Command);
    str_cat(Command, Service);

    const int32_t Code_Result_DisableService = system(Command);

    if (Code_Result_DisableService != SUCCESS) {
        return (ERR_SERVICE);
    }

    return (SUCCESS);
}

// -------------------------------------------------------------------------------------------
// Okei, esta función es un poco más compleja pero solo hace estas 3 cosas importantes:
// 1. Obtiene una lista de servicios activos del usuario en su maquina.
// 2. Compara esa lista de servicios activos con el servicio del usuario.
// 3. Y habiendo verificado que el servicio existe entonces lo desactiva.
//
// Lo demás son gestion de errores de manera simple, llama a la función que 
// muestra los errores al usuario y le facilita el trabajo al cliente donde solo
// recibe el servicio de desactivar un servicio.
//
// Ya te digo, esta función va enfocada directamente a la interacción con el usuario.
// No hace nada más porque para eso ya existe un script en bash que automatiza
// la desactivación estandarizada de servicios inutiles.
//
// Se retorna false para detener el programa por si ocurre un error en especifico
// que es grave y que deberia leer el mensaje de error para saber que hacer en esa
// situación.
// -------------------------------------------------------------------------------------------
const bool DISABLE_SERVICE(char *Service) {

    char List_Services[MAX_RANGE_SERVICES][MAX_RANGE_SERVICE_NAME];
    uint16_t Number_Services = 0;

    const uint8_t Code_Result_GetServices = GET_SERVICES(List_Services, &Number_Services);

    if (Code_Result_GetServices != SUCCESS) {
        Verification_ERR_SERVICES(Code_Result_GetServices);
        return (true);
    }


    bool Equal_Name_Services = false;

    for (uint8_t i = 0; i < Number_Services; i++) {
        if (str_cpm(List_Services[i], Service)) {
            Equal_Name_Services = true;
            break;
        }
    }

    if (!Equal_Name_Services) {
        Verification_ERR_SERVICES(ERR_SERVICE_NOT_EQ);
        return (true);
    }

    const uint8_t Code_Result_DisableService = Disable_Service(Service);

    if (Code_Result_DisableService == ERR_POPEN) {
        Verification_ERR_SERVICE(ERR_POPEN);
        return (false);
    }

    if (Code_Result_DisableService != SUCCESS) {
        Verification_ERR_SERVICES(Code_Result_DisableService);
        return (true);
    }

    return (true);
}
