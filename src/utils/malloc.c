/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Header for malloc functions
*/

#include "malloc.h"

//tempo pour les test de malloc
#include "screen.h"
void strdup(char *str, char *dest)
{
    int i = 0;
    for (; str[i]; i++){
        dest[i] = str[i];
    }
    dest[i] = 0;
}

int my_strlen(char const *str)
{
    int i;

    for (i = 0; str[i]; i++);
    return (i);
}

int my_put_nbr(int nb)
{
    int count = 100000000;
    int len = 0;
    nb < 0 ? nb *= -1, write_screen("-", 1), len++, 1 : 0;

    while (nb / count == 0 && count > 1)
        count = count / 10;

    while (count != 0){
        char a = 48 + nb / count;
        write_screen(&a, 1);
        len++;
        nb -= (nb/count) * count;
        count = count / 10;
    }
    return (len);
}

char *my_putnbr_base(unsigned long int nbr, char *base)
{
    int len = my_strlen(base);
    int i;
    static char to_print[65];
    static char to_print2[65];
    //nbr < 0 ? nbr *= -1 : 0;
    for (int y = 0; y < 65; to_print[y] = 0, to_print2[y] = 0, y++);
    for (len = 0; base[len]; len++);
    for (i = 0; nbr != 0; i++){
        to_print[i] = base[nbr%len];
        nbr /= len;
    }
    int a = 0;
    for (i = i-1; i >= 0; i--, a++)
        to_print2[a] = to_print[i];
    to_print2[0] == 0 ? to_print2[0] = base[0] : 0;
    return (to_print2);
}
//print memory
void mem_print(int x, int y, void *start, int size)
 {
    char *c_start = (char *)start;
    int xx = x;
    char hexa[] = "0123456789ABCDEF";
    static char str[2];
    str[1] = 0;
    int x2 = x;
    x--;
    y--;
    for (int i = 0; i < size/sizeof(char); i++){
        if (!(i % 8))
            y++, x = xx, x2 = x;
        if (!(i % 2))
            x++;
        char c = c_start[i];
        str[0] = hexa[c & 0x0F];
        mvprint(x, y, str, 0x07);
        str[0] = hexa[c >> 4 & 0x0F];
        mvprint(x+1, y, str, 0x07);
        str[0] = c;
        mvprint(x2+21, y, str, 0x09);
        x2++;
        x += 2;
    }
 }
/*void mem_print(int x, int y, void *start, int size)
{
    short *c_start = (short *)start;
    int xx = x;
    char *screen = OS_S_SCREEN;

    x--, y--;
    for (int i = 0; i < size/sizeof(int); i++){
        if (i % 4 == 0)
            y++, x = xx;
        else
            x++;
        short nbr = c_start[i];
        screen[((x-xx)/2+xx+20+y*CONSOLE_WIDTH)*2] = nbr;
        screen[((x-xx)/2+xx+20+y*CONSOLE_WIDTH)*2+1] = 0x07;
        screen[((x-xx)/2+xx+21+y*CONSOLE_WIDTH)*2] = nbr/256;
        screen[((x-xx)/2+xx+21+y*CONSOLE_WIDTH)*2+1] = 0x07;
        char *str = my_putnbr_base(nbr, "0123456789ABCDEF");
        for (int j = 0; my_strlen(str) + j < sizeof(int); j++, x++)
            mvprint(x, y, "0", 0x07);
        mvprint(x, y, str, 0x07);
        x += my_strlen(str);
    }
}*/


//end tempo

typedef struct mal{
    struct mal *prev;
    struct mal *next;
    long int size;
} mal_t;

struct malloc_data{
    void *end;
    mal_t *mal_mov;
};

struct malloc_data *data;

/*
[end, mal_mov, next, prev, size, 0, 0, 0]

*/
int init_malloc(void *mem_start, void *mem_end)
{
    data = (struct malloc_data *)mem_start;
    data->end = mem_end;
    mal_t *mal_o = (mal_t *)(data + sizeof(struct malloc_data));
    data->mal_mov = mal_o;
    mal_o->prev = 0;
    mal_o->next = mem_end;
    mal_o->size = 0;
    return (0);
}

void malloc_print2(void *p, int x, int y)
{
    char *str = my_putnbr_base((long int)p, "0123456789ABCDEF");
    int j = 0;
    for (; my_strlen(str) + j < 8; j++)
        mvprint(x+j, y, "0", 0x07);
    mvprint(x+j, y, str, 0x07);
    mvprint(x+8, y, " ", 0x07);
}

void malloc_print(int x, int y, void *p)
{
    mal_t *mov = (mal_t *)((char *)p);// - sizeof(mal_t));
    malloc_print2(mov, x, y);
    x += 9;
    malloc_print2(mov->prev, x, y);
    x += 9;
    malloc_print2(mov->next, x, y);
    x += 9;
    malloc_print2((void *)mov->size, x, y);
    x += 9;
}

void malloc_list(int x, int y)
{
    mal_t *mov = (mal_t *)(data + sizeof(struct malloc_data));
    mvprint(x, y, "  adr      prev     next     size ", 0x07);
    for (; mov->next; mov = mov->next){
        y++;
        malloc_print(x, y, mov);
    }
}


void *malloc(unsigned int size)
{
    static int db_line = -1;
    db_line++;
    mal_t *mov = data->mal_mov;

    for (; mov->size != 0; mov = mov->next){
    }
    //mvprint(20*db_line, 10, my_putnbr_base(data->mal_mov, "0123456789ABCDEF"), 0x09);
    //mvprint(20*db_line, 11, my_putnbr_base(sizeof(void *), "0123456789"), 0x09);
    unsigned int block_size = size + sizeof(mal_t);
    if ((char *)data->end - ((char *)mov > (char *)block_size)){
        void *p = (void *)((char *)mov + sizeof(mal_t));
        mov->size = size;
        mov->next = p + size;
        data->mal_mov = mov->next;
        mov->next->prev = mov;
        mov = mov->next;
        mov->size = 0;
        mov->next = 0;

        return (p);
    } else {
        return (0);
    }
    return (0);
}

long int malloc_get_id(void)
{
    static long int id = 0;
    id++;
    return (id);
}

int free(void *p)
{
    mal_t *mov = (mal_t *)((char *)p - sizeof(mal_t));
    if (mov->prev->size == 0){
        mov->prev->next = mov->next;
        mov->next->prev = mov->prev;
    } else {
        mov->size = 0;
    }
    return (0);
}
