
#ifndef _STRING_H
#define _STRING_H

#include <stdint.h>

int strlen(const char *str);
char *strstr (const char *head, const char *search);
char *strchr(const char *s, int c);
int strncmp(char const *str1, char const *str2, int nb);
void strcat(char *dest, char const *src);

void *memset(void *str, char c, size_t n);
void *memcpy(void *dest, void *src, size_t n);

#endif
