##
## EPITECH PROJECT, 2019
## SHIELD_OS
## File description:
## Makefile
##

SRC	=	src/kernel.c			\
		src/hardware/segments.c		\
		src/hardware/interrupts.c	\
		src/utils/mem.c			\
		src/utils/screen.c		\
		src/utils/string.c 		\
		src/utils/malloc.c \
		src/utils/lld/lld_db.c \
		src/utils/lld/lld_free.c \
		src/utils/lld/lld_init.c \
		src/utils/lld/lld_insert_node.c \
		src/utils/lld/lld_insert.c \
		src/utils/lld/lld_len_db.c \
		src/utils/lld/lld_len.c \
		src/utils/lld/lld_pop_node.c \
		src/utils/lld/lld_pop.c \
		src/utils/lld/lld_print_int.c \
		src/utils/lld/lld_print_str.c \
		src/utils/lld/lld_read.c \
		src/utils/lld/lld_sort.c \
		src/utils/lld/lld_write.c

OBJ	=	$(SRC:.c=.o)

ASM	=	builder/pointer.s
OBJ_S	=	builder/pointer.o

LINKER	=	builder/linker.ld

NAME	=	os_s

ISO_NAME=	system.iso
ISO_DIR	=	iso

TAB_NAME=	OS S v1.17.0
GRUB_CFG=	$(ISO_DIR)/boot/grub/grub.cfg

CFLAGS	=	-m32 -Wall -fno-stack-protector -nostdinc -ffreestanding -Iinclude
ASFLAGS	=	--32
LDFLAGS	=	-m elf_i386
VMFLAGS	=	-m 512M -display sdl

all:	$(NAME)

$(NAME):$(OBJ) $(OBJ_S)
	ld $(LDFLAGS) $(LINKER) -o $(NAME) $(OBJ_S) $(OBJ)

clean:
	rm -f  $(OBJ)
	rm -f  $(OBJ_S)
	rm -f  $(ISO_NAME)
	rm -rf $(ISO_DIR)

fclean:	clean
	rm -f $(NAME)
	$(MAKE) -C bootloader fclean --no-print-directory

re:	fclean all

vm:	iso
	qemu-system-i386 $(VMFLAGS) -cdrom $(ISO_NAME)

iso:	fclean $(NAME)
	mkdir -p $(ISO_DIR)/boot/grub
	$(MAKE) grub_cfg --no-print-directory
	cp $(NAME) $(ISO_DIR)/boot/
	grub2-mkrescue -o $(ISO_NAME) iso

grub_cfg:
	rm -f $(GRUB_CFG)
	echo -e "menuentry \"$(TAB_NAME)\" {" >> $(GRUB_CFG)
	echo    "    multiboot /boot/$(NAME)" >> $(GRUB_CFG)
	echo    "}" 			      >> $(GRUB_CFG)

install:all
	$(MAKE) -C bootloader install --no-print-directory
	sudo cp $(NAME) /boot/
