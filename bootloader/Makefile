##
## EPITECH PROJECT, 2019
## SHIELD_OS
## File description:
## Bootloader makefile
##

SRC	=	efi.c
OBJ	=	$(SRC:.c=.o)
SO	=	bootloader.so
NAME	=	shield.efi

CFLAGS	=			\
      -c			\
      -fno-stack-protector	\
      -fpic			\
      -fshort-wchar		\
      -mno-red-zone		\
      -I /lib/gnu-efi/inc	\
      -I /lib/gnu-efi/inc/x86_64\
      -DEFI_FUNCTION_WRAPPER

LDFLAGS	=						\
     /lib/gnu-efi/x86_64/gnuefi/crt0-efi-x86_64.o	\
     -nostdlib						\
     -znocombreloc					\
     -T /lib/gnu-efi/gnuefi/elf_x86_64_efi.lds		\
     -shared						\
     -Bsymbolic						\
     -L /lib/gnu-efi/x86_64/gnuefi			\
     -L /lib/gnu-efi/x86_64/lib				\
     -l:libgnuefi.a					\
     -l:libefi.a					\

OBJFLAGS=			\
	-j .text                \
	-j .sdata               \
        -j .data                \
        -j .dynamic             \
        -j .dynsym              \
        -j .rel                 \
        -j .rela                \
        -j .reloc               \
        --target=efi-app-x86_64

all:	$(OBJ)
	ld $(OBJ) $(LDFLAGS) -o $(SO)
	objcopy $(OBJFLAGS) $(SO) $(NAME)

clean:
	rm -f $(OBJ)
	rm -f $(SO)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

install:all
	sudo cp $(NAME) /boot/

uninstall:
	sudo rm -f /boot/$(NAME)
