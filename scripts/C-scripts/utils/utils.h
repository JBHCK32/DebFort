#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// ++++++++++++
// utils.c:
// ++++++++++++
size_t str_len(char *str);

bool str_num(char *str);

uint16_t Casting_Chars_Numbers(char *Char_Cast, uint8_t Size_Char);

bool str_cmp(char *str1, char *str2);

void str_cpy(char *Str_Origin, char *Str_Copy);

void str_cat(char *Str_Origin, char *Str_Cat);

#endif

