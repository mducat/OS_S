##
## EPITECH PROJECT, 2019
## SHIELD_OS
## File description:
## Makefile
##

SRC			=		src/kernel.c				\
												\
					src/hardware/interrupts.c	\
					src/hardware/segments.c		\
												\
					src/memory/malloc.c			\
					src/memory/mem.c			\
												\
					src/screen/screen.c			\
												\
					src/utils/string.c			\
												\
					src/lld/lld_db.c			\
					src/lld/lld_free.c			\
					src/lld/lld_init.c			\
					src/lld/lld_insert.c		\
					src/lld/lld_insert_node.c	\
					src/lld/lld_len.c			\
					src/lld/lld_len_db.c		\
					src/lld/lld_pop.c			\
					src/lld/lld_pop_node.c		\
					src/lld/lld_print_int.c		\
					src/lld/lld_print_str.c		\
					src/lld/lld_read.c			\
					src/lld/lld_sort.c			\
					src/lld/lld_write.c			\

OBJ			=		$(SRC:.c=.o)

# not needed for now
ASM			=		
OBJ_S		=		

LINKER		=		builder/linker.ld

NAME		=		OS_S_kernel

ISO_NAME	=		system.iso
ISO_DIR		=		iso
FAT			=		system.img
BIOS		=		bootloader/OVMF.fd

INC			= 		-Iinclude -Ibootloader/inc

CFLAGS		=		$(INC) -Wall -Wextra 				\
					-fno-stack-protector -nostdinc		\
					-ffreestanding -fpie -nostdlib

LDFLAGS		=		-pie

all:	$(NAME)

$(NAME):$(OBJ) $(OBJ_S)
	ld $(LDFLAGS) $(LINKER) -o $(NAME) $(OBJ) #$(OBJ_S)

clean:
	find . -name "*~" -delete -o -name "#*#" -delete
	rm -f  $(OBJ)
	rm -f  $(OBJ_S)
	rm -f  $(FAT)
	rm -f  $(ISO_NAME)
	rm -rf $(ISO_DIR)

fclean:	clean
	$(MAKE) -C bootloader fclean
	rm -f $(NAME)

re:	fclean all

vm:	iso
	qemu-system-x86_64 -bios $(BIOS) -cdrom $(ISO_NAME) -serial stdio

monitor:	iso
	qemu-system-x86_64 -bios $(BIOS) -cdrom $(ISO_NAME) -monitor stdio

# this one is accessible through gdb with gdb -ex 'target remote localhost:1234'
# does not start CPU at startup
debug: iso
	qemu-system-x86_64 -bios $(BIOS) -cdrom $(ISO_NAME) -monitor stdio -S -s

iso:	dos
	mkdir -p $(ISO_DIR)
	mv $(FAT) $(ISO_DIR)
	xorriso -as mkisofs -R -f -e $(FAT) -no-emul-boot -o $(ISO_NAME) $(ISO_DIR) 2> /dev/null

dos:	all
	$(MAKE) -C bootloader --no-print-directory
	dd if=/dev/zero of=$(FAT) bs=1k count=1440 2> /dev/null
	mformat -i $(FAT) -f 1440 ::
	mmd -i $(FAT) ::/efi
	mmd -i $(FAT) ::/efi/boot
	mmd -i $(FAT) ::/efi/boot/OS_S
	mcopy -i $(FAT) bootloader/bootx64.efi ::/efi/boot/bootx64.efi
	mcopy -i $(FAT) $(NAME) ::/efi/boot/OS_S/$(NAME)
	mattrib -i $(FAT) -a ::/efi/boot/OS_S/$(NAME)

install:all
	sudo mkdir -p /boot/efi/EFI/OS_S
	$(MAKE) -C bootloader install
	sudo cp $(NAME) /boot/efi/EFI/OS_S/

uninstall:
	$(MAKE) -C bootloader uninstall
	sudo rm -f /boot/efi/EFI/OS_S/$(NAME)
