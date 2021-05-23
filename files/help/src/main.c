
#include <oss.h>
#include <lld.h>

int main(void)
{
    write("OSS terminal. commands:\n");
    write("help: display this message.\n");
    write("cat: concatenates files into display.\n");
    write("ls: list files.\n");

    int a[] = {12, -412, 4321, -5678, 012, 0xFF, 0};
    char *strings[] = {"Hey", "Test !", "oijgeoj", "a", "re !", "azezf", 0};
    printf("% d, %+d, % +d, %+d\n", 5, 6, 7, 8);
    printf("%z\n", 123456);
    printf("%0#6.5lld%17s\n", 1234, "astekmoulinette");
    for (int i = 0; a[i] != 0; i++)
        printf("%6d\n", a[i]);

    unsigned long long int ulld = 12345678765432;
    printf("%Lu\n", ulld);

    float test = 5.41;
    int last_printfinted = 0;

    printf("%.1f %5.2p %5.2p%n\n", test, strings, 0, &last_printfinted);
    printf("last : %d\n", last_printfinted);

    printf("|%# 0+-10.6s|\n", strings[0]);
    printf("|%10.6s|\n", strings[1]);
    printf("|%10.6s|\n", strings[2]);
    printf("|%-10.6s|\n", strings[3]);
    printf("|%-10.6s|\n", strings[4]);
    printf("|%10.6s|\n", strings[5]);
    
    printf("|%-10.6s|\n%n", strings[6], &last_printfinted);
    printf("last : %d\n", last_printfinted);

    /*printf("%20llu\n", -5);
    printf("%# hx, %x, %# X\n", 123, 123, 123);
    printf("%#- 5o, %o\n", 1234, 1234);

    int fahrenheit;
    for (fahrenheit = 0; fahrenheit <= 300; fahrenheit += 20)
    printf("%3d %- 4d\n", fahrenheit, (int)((5.0/9.0)*(fahrenheit-32)));

    printf("The color: %s\n", "blue");
    printf("First number: %d\n", 12345);
    printf("Second number: %04d\n", 25);
    printf("Third number: %i\n", 1234);
    printf("Float number: %3.2f\n", 3.14159);
    printf("Hexadecimal: %x\n", 255);
    printf("Octal: %o\n", 255);
    printf("Unsigned value: %u\n", 150);
    printf("Just printfint the percentage sign %%\n", 10);

    printf("%-5d|\n", -5);

    printf(":%s:\n", "Hello, world!");
    printf(":%15s:\n", "Hello, world!");
    printf(":%.10s:\n", "Hello, world!");
    printf(":%-10s:\n", "Hello, world!");
    printf(":%-15s:\n", "Hello, world!");
    printf(":%.15s:\n", "Hello, world!");
    printf(":%15.10s:\n", "Hello, world!");
    printf(":%-15.10s:\n", "Hello, world!");

    printf("%- +#5.2c\n", 'a');
    printf("%-3.6b\n", 123456765);

    char *long_pointer[] = {"Hey"};
    printf("%%p = %p, %%#llx = %#llx\n", long_pointer, long_pointer);*/

    
    refresh();
    return 0;
}
