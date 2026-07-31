#include "services-uninstall.h"
#include <stdio.h>

#define LINE 1024
#define MAX_RANGE_LINE (LINE + 1)



// ------------------------------------------------------------------------------------------
// Para mi es codigo muy malo, pero no quiero replicar la libreria string.h,
// solo queria una función que hiciera exactamente lo que necesitaba y que confia
// en los datos exteriores, quiero decir esta función la uso unicamente
// en una sola linea de codigo, entonces no vea más utilidad y no veo la necesidad
// de gastar tiempo en mejorar algo que puedo hacer de manera simple.
//
// Al final lo hice porque se ajusta a mis necesidades y esta función basica
// las cumple y eso me basta. No es necesario hacerla más compleja, ni 
// preocuparse por la seguridad a la hora de manipular strings, porque en este
// caso en concreto no es necesaria, en mi codigo se utiliza solo para una cosa
// y eso es todo, el comando incluso que se obtiene el input esta harcodeado,
// no interactua con el usuario, no hace interacciones peligrosas, hace lo que tiene
// que hacer y listo, es publica lo cual si importas esta libreria y usas la función
// en algo para lo cual no fue hecha, al ser tan basica puede que al jugar con poco
// con ella se llegue a romper y ocasionar errores, pero es normal
// porque trata de ser especifica como la acción de respirar, no trata de ser un cuerpo
// completo que gestione todos los casos de riesgo.
// ------------------------------------------------------------------------------------------
static bool s_scanf(char *Str_Copy, char *Str_Paste) {
    size_t i = 0;

    while(Str_Copy[i] != '\0' && Str_Copy[i] != ' ') {
        Str_Paste[i] = Str_Copy[i];
        i++;
    }

    Str_Paste[i] = '\0';

    return (true);
}


// -------------------------------------------------------------------------------------------------
// Esta función de manera general ejecuta un comando que no necesita super 
// usuario para obtener el nombre de los servicios que tiene activos el usuario en el momento
// de ejecución.
// -------------------------------------------------------------------------------------------------
static uint8_t Get_Services(FILE **Container, char List_Services[][MAX_RANGE_NAME_SERVICES], uint16_t *Number_Services) {

    char line[MAX_RANGE_LINE];

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Ejecutamos systemctl y leemos la salida.
    //
    // Esto es necesario unicamente para obtener de manera
    // cruda de la salida del comando para ver que servicios retorna
    // y mostrarselos al usuario los servicios correspondientes
    // que corren en su maquina.
    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    *(Container) = popen("systemctl list-units --type=service --state=running --no-legend --no-pager", "r");
    
    if (*(Container) == NULL) {
        return (ERR_POPEN);
    }

    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Leemos lo que nos devolvio el comando con el tamñano
    // de MAX_RANGE_LINE para ir leyendo con ese buffer en la variable
    // line.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    while (fgets(line, sizeof(line), *(Container)) != NULL && *Number_Services < MAX_RANGE_SERVICES) {
        
        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Extraemos la primera palabra que es el nombre del servicio
        // del cual queremos mostrarle al usuario y esta activo en su maquina.
        // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        char service_name[MAX_RANGE_NAME_SERVICES];

        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        // Obtenemos la primera linea que veamos antes de un espacio usando s_scanf.
        // La idea es parsear la salida del comando con esa función que facilita la obtención
        // del nombre del servicio del usuario.
        // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
        if (s_scanf(line, service_name)) {
            str_cpy(List_Services[*Number_Services], service_name);
            Number_Services++;
        }
    }

    pclose(*(Container));

    return (SUCCESS);
} 

// --------------------------------------------------------------------------------
// Esta función muestra la lista de servicios de forma bonita al usuario, 
// es algo basico y que no se necesita mucho esfuerzo ya que solo hace una cosa, 
// mostrar el contenido de manera bonita de una variable que contiene
// la lista de servicios.
// --------------------------------------------------------------------------------
bool SHOW_SERVICES() {
    
    FILE *Output;

    uint16_t Number_Services = 0;

    char services[MAX_RANGE_SERVICES][MAX_RANGE_NAME_SERVICES];

    const uint8_t Code_Result_GetServices = Get_Services(&Output, services, &Number_Services);

    if (Code_Result_GetServices != SUCCESS) {
        Verification_ERR_SERVICES(Code_Result_GetServices);
        return (false);
    }

    printf("\n\x1b[1;32mServicios activos encontrados: %d\x1b[0m\n", Number_Services);
    for (int i = 0; i < Number_Services; i++) {
        printf("\x1b[1;36m->%d. %s\x1b[0m\n", i + 1, services[i]);
    }

    return (true);
}

// ---------------------------------------------------------------------------------
// Esta función es algo rapido para poder obtener la lista de
// servicios más reciente. Su utilidad recide en hacer verificaciones
// justo cuando se recibe el input del usuario y tener los datos más recientes
// a la hora de verificar.
// ---------------------------------------------------------------------------------
uint8_t GET_SERVICES(char List_Services[][MAX_RANGE_NAME_SERVICES], uint16_t *Number_Services) {
    
    FILE *flow_data;

    const uint8_t Code_Result_GetServices = Get_Services(&flow_data, List_Services, Number_Services);

    if (Code_Result_GetServices != SUCCESS) {
        return (Code_Result_GetServices);
    }

    return (SUCCESS);
}


