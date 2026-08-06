#include "utils.h"

size_t str_len(char *str) {
    size_t i = 0;

    while (str[i] != '\0') {i++;}

    return (i);
}

// -----------------------------------------------
// Esta función lo que hace de manera general
// es verificar si una cadena de texto tiene
// un caracter. Si llega a tener un caracter
// se devuelve de inmediato un valor falso
// porque esto verifica unicamente que la 
// cadena contenga numeros y no caracteres.
// ----------------------------------------------
bool str_num(char *str) {

    uint16_t i = 0;

    while(str[i] != '\0') {

        uint8_t char_cast = (uint8_t)str[i];

        if (char_cast < '0' || char_cast > '9') {
            return (false);
        }

        i++;
    }

    return (true);
}

// ------------------------------------------------------------
// Es una función matematica sencilla para numeros pequeños
// que saca la exponenciación de un numero.
// Es algo simple y ajustado a mis necesidades.
// ------------------------------------------------------------
static size_t math_pow(uint8_t base, uint8_t exponent) {

    size_t result = 1;

    for (uint8_t i = 1; i <= exponent; i++) {
        result *= base;
    }

    return (result);
}

// -----------------------------------------------
// Esta función pasa un numero de una cadena
// a un numero completo, lo hace con numeros
// pequeños pero es justo lo que necesito para
// el proyecto ya que solo se utiliza
// en la transforamción de un numero
// de puerto char a un número entero.
// -----------------------------------------------
uint16_t Casting_Chars_Numbers(char *Char_Cast, uint8_t Size_Char) {
     
    uint8_t i = 0;
    uint16_t Num_Cast = 0;

    while(Char_Cast[i] != '\0') {
        uint8_t Num = (uint8_t)Char_Cast[i] - 48;

        Num_Cast += Num * math_pow(10, (Size_Char - i));
        i++;
    }

    return (Num_Cast);
}

bool str_cmp(char *str1, char *str2) {
    
    size_t i = 0;

    while(str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return (false);
        }

        i++;
    }

    return (str1[i] == '\0' && str2[i] == '\0');
}

void str_cpy(char *Str_Origin, char *Str_Copy) {
    size_t i = 0;

    while(Str_Copy[i] != '\0') {
        Str_Origin[i] = Str_Copy[i];
        i++;
    }

    Str_Origin[i] = '\0';
}

void str_cat(char *Str_Origin, char *Str_Cat) {

    size_t i = 0;

    while(Str_Origin[i] != '\0') {i++;}

    size_t j = 0;

    while(Str_Cat[j] != '\0') {
        Str_Origin[i] = Str_Cat[j];
        i++;
        j++;
    }

    Str_Origin[i] = '\0';
}



