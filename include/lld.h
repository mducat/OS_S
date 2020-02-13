/*
** EPITECH PROJECT, 2020
** OS_S
** File description:
** Header for linked list
*/

#ifndef _LLD_
    typedef struct lld
    {
        void *data;
        struct lld *next;
        struct lld *prev;
    } lld_t;

    int lld_db(lld_t *lld);
    void lld_free(lld_t *lld);
    lld_t *lld_init();
    void lld_insert_node(lld_t *lld, int poss, lld_t *node);
    void lld_insert(lld_t *lld, int pos, void *data);
    int lld_len_db(lld_t *lld);
    int lld_len(lld_t *lld);
    void *lld_pop_node(lld_t *lld, int pos);
    void *lld_pop(lld_t *lld, int pos);
    void lld_print_int(lld_t *lld);
    void lld_print_str(lld_t *lld);
    void *lld_read(lld_t *lld, int poss);
    void lld_sort(lld_t *list, int (*cmp)());
    void *lld_write(lld_t *lld, int poss, void *data);
#endif
