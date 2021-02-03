
#include "malloc.h"

//tempo pour les test de malloc
#include "screen.h"
char *strdup(char *str)
{
    int i = 0;
    char *dest = malloc(my_strlen(str)+1);
    if (dest == 0)
        return (0);

    for (; str[i]; i++){
        dest[i] = str[i];
    }
    dest[i] = 0;
    return (dest);
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
    int xx = x-1;
    char hexa[] = "0123456789ABCDEF";
    static char str[2];
    str[1] = 0;
    int x2 = x;
    y--;
    for (unsigned long int i = 0; i < size/sizeof(char); i++){
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
    mal_t *mov = (mal_t *)((char *)data + sizeof(struct malloc_data));
    mvprint(x, y, "  adr      prev     next     size ", 0x07);
    for (; mov != data->end; mov = mov->next){
        y++;
        malloc_print(x, y, mov);
    }
    y++;
    malloc_print(x, y, mov);

}

int init_malloc(void *mem_start, void *mem_end)
{
    data = (struct malloc_data *)mem_start;
    mal_t *mal_o = (mal_t *)((char *)data + sizeof(struct malloc_data));
    data->mal_mov = mal_o;
    mal_o->prev = 0;
    mal_o->size = 0;
    mal_t *mal_end = (mal_t *)((char *)mem_end - sizeof(mal_t));
    data->end = mal_end;
    mal_o->next = mal_end;
    mal_o->prev = mal_end;
    mal_end->next = mal_o;
    mal_end->prev = mal_o;
    mal_end->size = 0;
    return (0);
}

void *malloc(int size)
{
    if (size <= 0) return (0);
    static int db_line = -1;
    db_line++;
    //mal_t *start = data->mal_mov;//speedy
    mal_t *start = (mal_t *)((char *)data + sizeof(struct malloc_data));//slowy
    mal_t *mov = start;

    unsigned int block_size = size + sizeof(mal_t)*2;
    for (; mov != start->prev; mov = mov->next){
        if ((long int)mov->next - (long int)mov - mov->size >= (long int)block_size){
            mal_t *new = (mal_t *)((char *)mov + sizeof(mal_t) + mov->size);
            new->next = mov->next;
            new->next->prev = new;
            new->prev = mov;
            new->size = size;
            mov->next = new;
            data->mal_mov = new;
            void *p = (void *)((char *)new + sizeof(mal_t));
            return (p);
        }
    }
    mvprint(35, db_line, "fail", 0x09);
    return (0);
}

int free(void *p)
{
    if ((char *)data + sizeof(mal_t) > (char *)p || (char *)p >= (char *)data->end){
        //mvprint(60, 0, "invalide free", 0x07);
        return (-1);
    }
    mal_t *mov = (mal_t *)((char *)p - sizeof(mal_t));
    if (data->mal_mov == mov)
    data->mal_mov = mov->prev;
    mov->prev->next = mov->next;
    mov->next->prev = mov->prev;
    return (0);
}

long int malloc_get_id(void)
{
    static long int id = 0;
    id++;
    return (id);
}
