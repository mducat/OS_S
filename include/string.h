
#ifndef OS_STRING_H
#define OS_STRING_H

int strlen(const char *str);
char *strstr (const char *head, const char *search);
char *strchr(const char *s, int c);
int strncmp(char const *str1, char const *str2, int nb);
void strcat(char *dest, char const *src);

#endif
