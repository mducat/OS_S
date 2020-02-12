/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Header for malloc functions
*/

#ifndef _MALLOC_
    #define MEM_START 0x100000
    #define MEM_END  0x8000000
    //133Mo de ram

//tempo
int my_strlen(char const *str);
void strdup(char *str, char *dest);
int my_put_nbr(int nb);
char *my_putnbr_base(int nbr, char *base);
void mem_print(int x, int y, void *start, int size);
//endtempo

void *malloc(unsigned int size);
int init_malloc(void *mem_start, void *mem_end);

#endif
