#include "utils.h"

size_t str_len(char *str) {
    size_t i = 0;

    while (str[i] != '\0') {i++;}

    return (i);
}
