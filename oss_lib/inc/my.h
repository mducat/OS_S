
#ifndef _my_h
#define _my_h

// main functions

void my_putchar(char) __attribute__((deprecated));
int my_isneg(int);
int my_put_nbr(int);
void my_swap(int *, int *);
int my_putstr(char const *) __attribute__((deprecated));
int my_strlen(char const *);
int my_getnbr(char const *);
void my_sort_int_array(int *, int);
int my_compute_power_rec(int, int);
int my_compute_square_root(int);
int my_is_prime(int);
int my_find_prime_sup(int);
char *my_strcpy(char *, char const *);
char *my_strncpy(char *, char const *, int);
char *my_revstr(char *);
char *my_strstr(char *, char const *);
int my_strcmp(char const *, char const *);
int my_strncmp(char const *, char const *, int);
char *my_strupcase(char *);
char *my_strlowcase(char *);
char *my_strcapitalize(char *);
int my_str_isalpha(char const *);
int my_str_isnum(char const *);
int my_str_islower(char const *);
int my_str_isupper(char const *);
int my_str_isprintable(char const *);
int my_showstr(char const *);
int my_showmem(char const *, int);
char *my_strcat(char *, char const *);
char *my_strncat(char *, char const *, int);
char *my_strdup(char const *src);
char **my_str_to_word_array(char const *);
int my_show_word_array(char * const *);
int is_num(char);

long strtol(const char *nptr, char **endptr, register int base);
char *strdup(const char *str);
int strcmp(const char *str1, const char *str2);
char *my_putnbr_base_str(unsigned long int nbr, char *base);
char **strToWords(const char *str, char split);
void memcpy(char *dest, const char *src, int size);
int strlen(const char *str);

//backend functions

int abs_rec(int);
int my_is_prime_finder(int);
int is_recognized(char);
void swap_rev(char *, char *);
int get_display_length_mem(int, char const *);
int call_one_mem(int *, int *, char *, char const *);
int my_putnbr_base_mem(int, char const *, int);
int my_showmem_part(int *, char const *);
int get_display_length_show(int, char const *);
int call_one_show(int *, int *, char *, char const *);
int my_putnbr_base_show(int, char const *);
int my_isprintable(char);
void swap_sort(int *, int *);
int is_upcase(char);
int is_lowcase(char);
int get_len_strstr(char *);
int get_index_strstr(char *, char const *);
int is_alphanumeric(char);
int count_occur(char const *);
char **malloc_array(char const *);

//my_printf

typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;

#define va_start(v,l)        __builtin_va_start(v,l)
#define va_end(v)        __builtin_va_end(v)
#define va_arg(v,l)        __builtin_va_arg(v,l)


typedef struct conf{
    int *flags;
    int width;
    int precision;
    int size;
    int char_printed;
} pf_conf_t;

int my_printf(char const *, ...);

void pf_putchar(char, pf_conf_t *);
void pf_putstr(char *, pf_conf_t *);
void pf_put_nbr(long long int, pf_conf_t *);
void handle_conf(char, pf_conf_t *, int, int);
void post_conf(char, pf_conf_t *, int, int);

void get_configuration(char const *format, int i, pf_conf_t *conf);

void get_flags(char const *format, int i, int fl[]);
int get_size(char const *format, int i);
int get_type(char const *format, int i);
int get_width(char const *format, int i);
int get_precision(char const *format, int i);

int get_flag_index(char const *format, int i);
int get_width_index(char const *format, int i);
int get_precision_index(char const *format, int i);
int get_length_index(char const *format, int i);

int is_numtype(char c);

void my_putnbr_base(unsigned long long int nbr, char const *base, int disp_len,
    pf_conf_t *conf);
void my_put_unsigned_nbr(unsigned long long int nb, pf_conf_t *conf);
int get_nbr_length(long long int nbr, char *base, int is_signed);
long long int get_number(va_list *, pf_conf_t *, int);

void print_str(va_list *, pf_conf_t *);
void print_str_format(va_list *, pf_conf_t *);
void print_nbr(va_list *, pf_conf_t *);
void print_char(va_list *, pf_conf_t *);
void print_pointer(va_list *, pf_conf_t *);
void print_binary(va_list *, pf_conf_t *);
void print_hexa(va_list *, pf_conf_t *);
void print_unsigned(va_list *, pf_conf_t *);
void print_percent(va_list *, pf_conf_t *);
void print_double(va_list *, pf_conf_t *);
void print_errno(va_list *, pf_conf_t *);
void print_memory(va_list *, pf_conf_t *);
void print_octal(va_list *, pf_conf_t *);
void print_hexa_maj(va_list *, pf_conf_t *);

// macros

#define abs(x)    ((x) < 0 ? -(x) : (x))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) > (b) ? (b) : (a))

#define __OSS__

#endif
