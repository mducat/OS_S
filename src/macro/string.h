/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Header for all string functions
*/

#ifndef OS_STRING_H
#define OS_STRING_H
#include "../utils/string.c"

int strlen(const char *str);
char *strstr (const char *head, const char *search);
char *strchr(const char *s, int c);
int strncmp(char const *str1, char const *str2, int nb);
void strcat(char *dest, char const *src);

#endif
