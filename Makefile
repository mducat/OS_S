##
## EPITECH PROJECT, 2019
## SHIELD_OS
## File description:
## Makefile
##

OBJ			=		src/kernel.o				\
												\
					src/hardware/interrupts.o	\
					src/hardware/segments.o		\
												\
					src/memory/malloc.o			\
					src/memory/mem.o			\
												\
					src/screen/screen.o			\
					src/screen/data.o			\
												\
					src/utils/string.o			\
												\
					src/lld/lld_db.o			\
					src/lld/lld_free.o			\
					src/lld/lld_init.o			\
					src/lld/lld_insert.o		\
					src/lld/lld_insert_node.o	\
					src/lld/lld_len.o			\
					src/lld/lld_len_db.o		\
					src/lld/lld_pop.o			\
					src/lld/lld_pop_node.o		\
					src/lld/lld_print_int.o		\
					src/lld/lld_print_str.o		\
					src/lld/lld_read.o			\
					src/lld/lld_sort.o			\
					src/lld/lld_write.o			\
												\
												\
												\
					src/hardware/int_handler.o	\

LINKER		=		builder/linker.ld

NAME		=		OS_S_kernel

ISO_NAME	=		system.iso
ISO_DIR		=		iso
FAT			=		system.img
BIOS		=		bootloader/OVMF.fd

INC			= 		-Iinclude -Ibootloader/inc

CFLAGS		=		$(INC) -Wall -Wextra 				\
					-fno-stack-protector -nostdinc		\
					-ffreestanding -fpie -nostdlib		\
					-mno-red-zone

LDFLAGS		=		-pie

VMFLAGS		=		-bios $(BIOS) -cdrom $(ISO_NAME)

all:	$(NAME)
	$(MAKE) -C bootloader --no-print-directory

$(NAME):$(OBJ)
	ld $(LINKER) $(LDFLAGS) -o $(NAME) $(OBJ)

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
	qemu-system-x86_64 $(VMFLAGS) -serial stdio

monitor:	iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio

# this one is accessible through gdb with gdb -ex 'target remote localhost:1234'
# does not start CPU at startup
debug: 	CFLAGS += -g
debug: 	iso
	qemu-system-x86_64 $(VMFLAGS) -monitor stdio -S -s

iso:	img
	mkdir -p $(ISO_DIR)
	mv $(FAT) $(ISO_DIR)
	xorriso -as mkisofs -R -f -e $(FAT) -no-emul-boot -o $(ISO_NAME) $(ISO_DIR) 2> /dev/null

img:	all
	dd if=/dev/zero of=$(FAT) bs=1k count=1440 2> /dev/null
	mformat -i $(FAT) -f 1440 ::
	mmd -i $(FAT) ::/efi
	mmd -i $(FAT) ::/efi/boot
	mmd -i $(FAT) ::/efi/OS_S
	mcopy -i $(FAT) bootloader/bootx64.efi ::/efi/boot/bootx64.efi
	mcopy -i $(FAT) $(NAME) ::/efi/OS_S/$(NAME)
	mattrib -i $(FAT) -a ::/efi/OS_S/$(NAME)

install:all
	sudo mkdir -p /boot/EFI/OS_S
	$(MAKE) -C bootloader install
	sudo cp $(NAME) /boot/EFI/OS_S/

uninstall:
	$(MAKE) -C bootloader uninstall
	sudo rm -f /boot/EFI/OS_S/$(NAME)
