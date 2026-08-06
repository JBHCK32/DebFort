#include "lib-gui.h"


// -----------------------------------------------------------------------
// Este es la función que ejecuta el menú interactivo con el usuario
// en donde se le pide al usuario el nombre del servicio que quiere
// desactivar solo de los que estan activos y se le muestran al usuario.
//
// Además se conecta con al libreria services-uninstall.h que ayuda 
// en el proceso de desactivar el servicio por medio de una API
// que hace que el servidor solo tenga que verifiar que todo salio bien.
//
// Tambien como se menciona en el banner unformativo antes de entrar
// a al menú se espera la letra 's' o 'S' para detener el bucle 
// y que vuelva al menú principal.
// -----------------------------------------------------------------------
void Services_Menu_Loop() {
    while (true) {
        

        sleep(2);
        clear();

        SHOW_SERVICES();

        char Service_User[MAX_RANGE_NAME_SERVICES];

        printf("\n\x1b[36mEscribe el nombre del servicio a desactivar: \x1b[0m");
        const uint8_t Code_Result_Input = Processing_InputChar(Service_User, MAX_RANGE_NAME_SERVICES);

        if (Code_Result_Input == ERR_SIGINIT) {
            Message_ERR();
            continue;
        }
    
        else if (Code_Result_Input != SUCCESS) {
            Verification_ERR_MENU(Code_Result_Input);
            continue;
        }

        if ((Service_User[0] == 's' || Service_User[0] == 'S') && Service_User[1] == '\0') {
            printf("\n\x1b[36m>> Saliendo de la configuración de servicios...\x1b[0m\n");
            return;
        }

        bool Service_Disable = DISABLE_SERVICE(Service_User);

        if (!Service_Disable) {
            return;
        }
    }
}

// -------------------------------------------------------------------
// Esta es la función que ejecuta el menú de cerrado de puertos.
// No se cierran en terminos fisicos como lo hace un firewall como 
// iptables, sino que los cierra de manera interna en la maquina.
//
// Usamos la libreria disable-services.h y usamos la API que 
// facilita todo al cliente para que unicamente revise que el proceso
// haya salido bien.
// -------------------------------------------------------------------
void Ports_Menu_Loop() {
    
    while (true) {
        sleep(2);
        clear();

        printf("\n\x1b[32m--------------------------------------------\x1b[0m\n");
        int32_t Code_Result_ShowPorts = system("sudo ./scripts/shell-scripts/show-ports.sh");

        if (Code_Result_ShowPorts != SUCCESS) {return;}
        printf("\n\x1b[32m--------------------------------------------\x1b[0m\n");

        char Port_User[MAX_LEN_PORT];

        printf("\n\x1b[36mEscribe el numero del puerto a desactivar: \x1b[0m");
        const uint8_t Code_Result_Input = Processing_InputChar(Port_User, MAX_LEN_PORT);

        if (Code_Result_Input == ERR_SIGINIT) {
            Message_ERR();
            continue;
        }
    
        else if (Code_Result_Input != SUCCESS) {
            Verification_ERR_MENU(Code_Result_Input);
            continue;
        }

        if ((Port_User[0] == 's' || Port_User[0] == 'S') && Port_User[1] == '\0') {
            printf("\n\x1b[36m>> Saliendo del menú de puertos...\x1b[0m\n");
            return;
        }

        bool Number_Port = str_num(Port_User);

        if (!Number_Port) {
            Verification_ERR_MENU(ERR_INVALID_PORT);
            continue;
        }

        uint16_t PortUser_Casting = Casting_Chars_Numbers(Port_User, LEN_PORT);

        bool Close_PortUser = CLOSE_PORT_TO_MENU(PortUser_Casting);

        if (!Close_PortUser) {
            return;
        }
    }
} 









// ------------------------------------------------------------------
// Estas son las macros que definen el maximo numerico
// de la configuración de reglas de contraseña.
//
// Se utilizan para comparar de manera correcta si el usuario
// haya escrito de manera correcta el valor de esa configuración
// en su rango permitido.
// ------------------------------------------------------------------
#define MAX_LEN_PASSWORD 32
#define MAX_DAYS 120
#define MAX_WARN 14
#define MAX_HISTORY 3
#define MAX_CLASS 5
#define MAX_DIGITS 3
#define MAX_UPPER 3
#define MAX_LOWER 3
#define MAX_SPECIAL 3

// --------------------------------------------------------------------------
// Estas macros definen el valor minimo que puede escribir y tener
// el usuario en su configuración de reglas de contraseña.
//
// Sirve unicamente para ver si el usuario escribio un valor 
// que sea debajo de ese minimo para establecerlo de manera automatica
// en el minimo posible o en su contrario en el maximo posible
// con las macros que declaran el maximo valor numerico de
// la configuración de reglas de contraseña.
// --------------------------------------------------------------------------
#define MINIMUN_LEN_PASSWORD 12
#define MINIMUN_DAYS 90
#define MINIMUN_WARN 4
#define MINIMUN_HISTORY 1
#define MINIMUN_CLASS 3
#define MINIMUN_DIGITS 1
#define MINIMUN_UPPER 1
#define MINIMUN_LOWER 1
#define MINIMUN_SPECIAL 1

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Macros de los rangos o longitud de las variabes de buffer
// donde se obtiene el input del usuario para la configuración de politicas
// y privilegios.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MIN_LEN 2
#define RANGE_MIN_LEN (MIN_LEN + 1)

#define MIN_DAYS 3
#define RANGE_MIN_DAYS (MIN_DAYS + 1)

#define MIN_WARN 2
#define RANGE_MIN_WARN (MIN_WARN + 1)

#define MIN_HISTORY 1
#define RANGE_MIN_HISTORY (MIN_HISTORY + 1)

#define MIN_CLASS 1
#define RANGE_MIN_CLASS (MIN_CLASS + 1)

#define MIN_DIGITS 1
#define RANGE_MIN_DIGITS (MIN_DIGITS + 1)

#define MIN_UPPER 1
#define RANGE_MIN_UPPER (MIN_UPPER + 1)

#define MIN_LOWER 1
#define RANGE_MIN_LOWER (MIN_LOWER + 1)

#define MIN_SPECIAL 1
#define RANGE_MIN_SPECIAL (MIN_SPECIAL + 1)



// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Estos son macros para calcular el rango/longitud que va a tener
// el comando que ejecutara el script de configuración de politicas y permisos.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define MAX_RANGE_CONFIG_LIST 9

#define INIT_COMMAND 48
#define MAX_RANGE_INIT_COMMAND (INIT_COMMAND + 1)

#define MAX_RANGE_COMMAND (INIT_COMMAND + MIN_DAYS + MIN_WARN + MIN_HISTORY + MIN_CLASS + MIN_DIGITS + MIN_UPPER + MIN_LOWER + MIN_SPECIAL + 1)


// ------------------------------------------------------------------------------------------------------------------------
// Okei esta función interactua con el usuario por medio de Processing_InputChar, procesa lo que el usuario escribio
// y verifica que sea un número que no sobre pase el maximo ni el minimo permitido en la configuración, esos
// valores estan definidos con macros y son los que mantienen una coherencia en la ejecución de toda la función.
//
// La función hace algo sencillo, transforma los numeros en strings para guardarlos en el buffer que pase el usuario
// y ahi es donde se guardara el dato donde se aplicara después en la configuración.
// ------------------------------------------------------------------------------------------------------------------------
// 
// > Que hace estas función?:
// 
// 1.Obtenemos el input el usuario de manera segura:
//
// >> Code:
//
// const uint8_t Code_Result_Input_User = Processing_InputChar(BUFFER, RANGE_MIN);
//
// Esta parte es algo estandarizada, pero en general usamos
// esa función para que lo que llegue al buffer sea unicamente
// del tamaño que le pedimos, sino nos devuelve un error si ocurre
// un imprevisto y lo procesamos con la salida de ese mismo error.
//
// 2. Verficamos el '\0' en el input del usuario:
// 
//  >> Code:
//
//  if (BUFFER[0] == '\0');
//  
// Esta verificación es para aplicar la automatización
// de que el usuario solo le de enter en la opción
// y que se aplique el maximo de manera automatica.
//
// Aqui verificamos el finalizador de cadena porque lo que hace la función
// Processing_InputChar es justamente reemplazar el salto de linea por un
// finalizador de cadena o mejor conocido como '\0' y por eso lo verificamos
// en vez de un salto de linea normal que dejaria un ENTER.
//
// 3. Verificación antes de castear el input del usuario:
//
// >> Code:
//
// if (!str_num(BUFFER));
//
// Aqui es obvio que verificamos que sea un numero lo que haya escrito el usuario,
// pero lo verificamos para no procesar cualquier cosa, para delimitar el comportamiento
// correcto del usuario negamos su entrada aleatoria de caracteres y le notificamos 
// que solo se aceptan numeros.
// 
//
// 4. Verificación de que el input del usuario este en el rango permitido:
//
// >> Code:
// 
// if (Input_Number < MINIMUN_NUMBER);
// 
// else if (Input_Number > MAX_NUMBER);
//
// Después de castear lo escrito por el usuario y obtener el número correspondiente
// de lo que escribio, verificamos que lo que haya escrito se encuentre en el rango 
// pre-establecido en los parametos de la función, donde se establece mediante 
// un rango minimo y otro rango maximo.
//
// Naturalmente estos parametros se suelen pasar mediante macros 
// definidas para que estos rangos sean coherentes con lo permitido en la configuración 
// del script y de la seguridad de la maquina en general.
//
// Si quisieras modificar unicamente el parametro para modificar el minimo o el maximo
// deberias modificar la macros que tiene en su nombre MINIMUN o MAX, pero igual son
// faciles de encontrar porque hay un comentario encima de ellas explicando su función.
//
// 5. Por ultimo si el input del usuario es correcto entonces se agrega su formato numerico:
//
// >> Code:
//
// sprintf(BUFFER, "%u", (unsigned int)Input_Number);
//
// Lo hago de esta manera ya que el usuario podria haber escrito 
// perfectamente "012" y al procesarlo de forma numerica con el 
// casteo pasa todas las pruebas pero se manda el string 
// con ese valor incorrecto, no lo quedeberia ser.
//
// Por eso mismo el sprintf() es crucial en ese caso ya que evita interpretaciones
// octales por parte del script de configuración o que esa opción se ponga en el
// minimo siempre por defecto en terminos del script al no recibir un parametro correcto.
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
static uint8_t Ask_Number(char *BUFFER, const uint8_t RANGE_MIN, const uint8_t MINIMUN_NUMBER, const uint8_t MAX_NUMBER, const char *TEXT_STEP) {
    
    printf("\n>> Ingrese Porfavor %s [max %u]: ", TEXT_STEP, (unsigned int)MAX_NUMBER);

    const uint8_t Code_Result_Input_User = Processing_InputChar(BUFFER, RANGE_MIN);

    if (Code_Result_Input_User == ERR_SIGINIT) {
        Message_ERR();
        return (ERR_SIGINIT);
    }
    
    else if (Code_Result_Input_User != SUCCESS) {
        Verification_ERR_MENU(Code_Result_Input_User);
        return (Code_Result_Input_User);
    }

    if (BUFFER[0] == '\0') {
        printf("\n\x1b[36m>> Se usó el valor máximo por defecto: %u\x1b[0m\n", (unsigned int)MAX_NUMBER);
        sprintf(BUFFER, "%u", (unsigned int)MAX_NUMBER);       
        return (SUCCESS);
    }

    if (!str_num(BUFFER)) {
        printf("\n\x1b[1;31m>> Debes ingresar solo números.\x1b[0m\n");
        return (ERR_INVALID_NUMBER);
    }

    const uint8_t Size_Buffer = str_len(BUFFER);
    const uint8_t Input_Number = (uint8_t)Casting_Chars_Numbers(BUFFER, Size_Buffer);

    if (Input_Number < MINIMUN_NUMBER) {
        printf("\n\x1b[33m>> El mínimo es %u. Se usará %u.\x1b[0m\n", (unsigned int)MINIMUN_NUMBER, (unsigned int)MINIMUN_NUMBER);
        sprintf(BUFFER, "%u", (unsigned int)MINIMUN_NUMBER);  
        return (SUCCESS);
    }

    else if (Input_Number > MAX_NUMBER) {
        printf("\n\x1b[33m>> El máximo es %u. Se usará %u.\x1b[0m\n", (unsigned int)MAX_NUMBER, (unsigned int)MAX_NUMBER);
        sprintf(BUFFER, "%u", (unsigned int)MAX_NUMBER);
        return (SUCCESS);
    }

    sprintf(BUFFER, "%u", (unsigned int)Input_Number);

    return (SUCCESS);
}

// ---------------------------------------------------------
// Estas son macros para definir el maximo de elementos
// en la lista de configuración de reglas de contraseña.
// ---------------------------------------------------------
#define LIST_RULES 9
#define MAX_RANGE_LIST_RULES (LIST_RULES + 1)

// ----------------------------------------------------------------------------
// Esta función es la que esambla y ejecuta el scritp de configuración.
//
// Bueno esto recibe una lista de strings, anteriormente trabajaba en el 
// flujo lineal pero moduralize la acción y ahora recibe un la lista
// con un rango rigido que se aplica para poder procesarla y esamblar
// el comando.
// ----------------------------------------------------------------------------
static void Execute_Script_Conf(char List_Conf[MAX_RANGE_LIST_RULES]) {
    char Command[MAX_RANGE_COMMAND];

    
    char Init_Command[MAX_RANGE_INIT_COMMAND] = "sudo ./scripts/shell-scripts/conf-privileges.sh ";

    str_cat(Command, Init_Command);

    for (uint8_t i = 0; i < MAX_RANGE_LIST_RULES; i++) {
        str_cat(Command, " ");
        str_cat(Command, List_Conf[i]);
    }

    printf("\n\x1b[36m>> Aplicando configuración...\x1b[0m\n");

    int ret = system(Command);

    if (ret == 0) {
        printf("\n\x1b[32m>> Configuración aplicada correctamente.\x1b[0m\n");
    }
}

// --------------------------------------------------------------------------------------------------------
// Como su nombre indica es un resumen de la configuración que se va a aplicar y sirve para informar
// al usuario, de paso sirve para que el usuario decida si volver hacer la configuración o seguir
// adelante con la misma.
// --------------------------------------------------------------------------------------------------------
static void Summary_Conf_Privileges(char List_Config[MAX_RANGE_LIST_RULES]) {
    clear();
    printf("\n\x1b[1;32m------------------------------------------------------------------------------------------\x1b[0m\n");
    printf("\x1b[1;36m RESUMEN DE CONFIGURACIÓN\x1b[0m\n");
    printf("\x1b[1;32m------------------------------------------------------------------------------------------\x1b[0m\n");
    printf("\x1b[36m  Longitud mínima           : %d\x1b[0m\n", List_Config[0]);
    printf("\x1b[36m  Caducidad (días)          : %d\x1b[0m\n", List_Config[1]);
    printf("\x1b[36m  Advertencia (días)        : %d\x1b[0m\n", List_Config[2]);
    printf("\x1b[36m  Historial                 : %d\x1b[0m\n", List_Config[3]);
    printf("\x1b[36m  Clases de caracteres      : %d\x1b[0m\n", List_Config[4]);
    printf("\x1b[36m  Dígitos                   : %d\x1b[0m\n", List_Config[5]);
    printf("\x1b[36m  Mayúsculas                : %d\x1b[0m\n", List_Config[6]);
    printf("\x1b[36m  Minúsculas                : %d\x1b[0m\n", List_Config[7]);
    printf("\x1b[36m  Especiales                : %d\x1b[0m\n", List_Config[8]);
    printf("\n\x1b[36m>> Presiona Enter para aplicar la configuración o 's'/'S' para cancelar:\x1b[0m");
}


// --------------------------------------------------------------------------------------------------------
// Este es el bucle que orquesta todo el menú de configuración de politicas de contraseñas.
// En permisos y grupos el script se encarga de manera automatica, creando un grupo admin y simplemente
// ejecutando los parametros que le pasemos.
// --------------------------------------------------------------------------------------------------------
void Privileges_Menu_Loop () {
    
    char buffer_len[RANGE_MIN_LEN];
    char buffer_days[RANGE_MIN_DAYS];
    char buffer_warn[RANGE_MIN_WARN];
    char buffer_history[RANGE_MIN_HISTORY];
    char buffer_class[RANGE_MIN_CLASS];
    char buffer_digits[RANGE_MIN_DIGITS];
    char buffer_upper[RANGE_MIN_UPPER];
    char buffer_lower[RANGE_MIN_LOWER];
    char buffer_special[RANGE_MIN_SPECIAL];
    
    while (true) {
        sleep(GENERIC_SLEEP);
        clear();

        // Longitud mínima
        if (Ask_Number(buffer_len, RANGE_MIN_LEN, MINIMUN_LEN_PASSWORD, MAX_LEN_PASSWORD, "la longitud de contraseña") != SUCCESS) {
            continue;
        }
        // Días de caducidad
        if (Ask_Number(buffer_days, RANGE_MIN_DAYS, MINIMUN_DAYS, MAX_DAYS, "los días de caducidad") != SUCCESS) {
            continue;
        }
        // Días de advertencia
        if (Ask_Number(buffer_warn, RANGE_MIN_WARN, MINIMUN_WARN, MAX_WARN, "los días de advertencia") != SUCCESS) {
            continue;
        }
        // Historial
        if (Ask_Number(buffer_history, RANGE_MIN_HISTORY, MINIMUN_HISTORY, MAX_HISTORY, "el historial de contraseñas") != SUCCESS) {
            continue;
        }
        // Clases de caracteres
        if (Ask_Number(buffer_class, RANGE_MIN_CLASS, MINIMUN_CLASS, MAX_CLASS, "la clases de caracteres") != SUCCESS) {
            continue;
        }
        // Dígitos
        if (Ask_Number(buffer_digits, RANGE_MIN_DIGITS, MINIMUN_DIGITS, MAX_DIGITS, "la cantidad de dígitos") != SUCCESS) {
            continue;
        }
        // Mayúsculas
        if (Ask_Number(buffer_upper, RANGE_MIN_UPPERY, MINIMUN_UPPER, MAX_UPPER, "la cantidad de mayúsculas") != SUCCESS) {
            continue;
        }
        // Minúsculas
        if (Ask_Number(buffer_lower, RANGE_MIN_LOWER, MINIMUN_LOWER, MAX_LOWER, "la cantidad de minúsculas") != SUCCESS) {
            continue;
        }
        // Especiales
        if (Ask_Number(buffer_special, RANGE_MIN_SPECIAL, MINIMUN_SPECIAL, MAX_SPECIAL, "la cantidad de caracteres especiales") != SUCCESS) {
            continue;
        }

        break;
    }

    // ------------------------------------------------------------------
    // Creación de la lista de configuración de reglas de contraseña:
    // ------------------------------------------------------------------
    char List_Conf_Rules_Pwd[MAX_RANGE_LIST_RULES] = {
        bufferr_len, 
        buffer_days, 
        buffer_warn, 
        buffer_history, 
        buffer_class, 
        buffer_digits, 
        buffer_upper, 
        buffer_lower, 
        buffer_special
    };
    
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Le mostramos un resumen informativo al usuario para que el mismo verifique si realmente
    // quiere esac onfiguración sino toma la desición de no hacerlo y volver hacer la configuración.
    // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    Summary_Conf_Privileges(List_Conf_Rules_Pwd);

    getchar();

    if (c == 's' || c == 'S') {
        printf("\n\x1b[1;31m>> Configuración cancelada.\x1b[0m\n");
        return;
    }

    
    Execute_Script_Conf(List_Conf_Rules_Pwd);    
}
