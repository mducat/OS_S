#include <stdlib.h>

#ifndef _ucp_h_
#define _ucp_h_

typedef long unsigned int u64;
typedef unsigned int u32;


#define IS_FLAG 0x8000000000000000
#define IS_SET(str) ((char *)((u64)str | (u64)0x8000000000000000))

int ucp(char *str, char *regex, ...);
char ***ucp_tab(char *str, char *regex, ...);
void ucp_free(char ***tab);

void *is_made1(char *str);
void *is_made2(char *str);
void *is_made3(char *str);
void *is_made4(char *str);
void *is_made5(char *str);

#endif

#include <stdlib.h>

//linked list (lld)
typedef struct lld
{
    void *data;
    struct lld *next;
    struct lld *prev;
} lld_t;

int lld_db(lld_t *lld);
void lld_free_r(lld_t *lld);
void lld_free(lld_t *lld);
lld_t *lld_init();
void lld_insert_node(lld_t *lld, int poss, lld_t *node);
void lld_insert(lld_t *lld, int pos, void *data);
int lld_len_db(lld_t *lld);
int lld_len(lld_t *lld);
void **lld_lld_to_tab(lld_t *lld);
void *lld_pop_node(lld_t *lld, int pos);
void *lld_pop(lld_t *lld, int pos);
void lld_print_int(lld_t *lld);
void lld_print_str(lld_t *lld);
void *lld_read(lld_t *lld, int poss);
void lld_sort(lld_t *list, int (*cmp)());
void *lld_write(lld_t *lld, int poss, void *data);
lld_t *lld_tab_to_lld(void **tab, int size);

char **strToWords(const char *str, char split);
char *my_putnbr_base_str(unsigned long int nbr, char *base);


