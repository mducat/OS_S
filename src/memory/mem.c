
int memset(char *str, char c, int size)
{
    int i = 0;

    for (; str[i] && i < size; i++) {
        str[i] = c;
    }
    return (0);
}

int memcpy(char *dest, char *src)
{
    //__try{ // Can't compile ?
    int x = 0;

    for (; src[x]; x++)
        dest[x] = src[x];
    dest[x] = 0;
    return (0);
    //}__except{
    //    return (84);
    //}
}
