/*
** EPITECH PROJECT, 2019
** SHIELD_OS
** File description:
** Kernel main file
*/

void write_screen(char *str)
{
    char *screen = (void *) 0xb8000;

    for (int i = 0; str[i] != '\0'; i++){
        screen[i * 2 + 1] = 0x09; // Color 0x9 = light blue
        screen[i * 2] = str[i];
    }
}

void kernel_main(void)
{
    write_screen("Hello, world !\n Re-hello !");
    write_screen("Test !");
}
