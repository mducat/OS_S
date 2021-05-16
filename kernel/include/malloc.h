
#ifndef _MALLOC_
    #define MEM_START ((void *) 0x100000)
    #define MEM_END  ((void *) 0x8000000)
    //133Mo de ram

    //tempo
    int my_strlen(char const *str);
    char *strdup(char *str);
    int my_put_nbr(int nb);
    char *my_putnbr_base(unsigned long int nbr, char *base);
    void mem_print(int x, int y, void *start, int size);
    void malloc_print(int x, int y, void *p);
    void malloc_list(int x, int y);
    //endtempo

    void *malloc(int size);
    int init_malloc(void *mem_start, void *mem_end);
    int free(void *);

#endif
