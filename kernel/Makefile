
OBJ			=		src/kernel.o                \
                                                \
					src/hw/io.o                 \
					src/hw/irq.o                \
					src/hw/gdt.o                \
                                                \
					src/hw/dev/kbd.o            \
					src/hw/dev/ps2.o            \
					src/hw/dev/mouse.o          \
					src/hw/dev/serial.o         \
                                                \
					src/memory/malloc.o         \
					src/memory/mem.o            \
                                                \
					src/screen/screen.o         \
					src/screen/data.o           \
                                                \
					src/utils/string.o          \
												\
					src/font/font.o				\
                                                \
					src/lld/lld_db.o            \
					src/lld/lld_free.o          \
					src/lld/lld_init.o          \
					src/lld/lld_insert.o        \
					src/lld/lld_insert_node.o   \
					src/lld/lld_len.o           \
					src/lld/lld_len_db.o        \
					src/lld/lld_pop.o           \
					src/lld/lld_pop_node.o      \
					src/lld/lld_print_int.o     \
					src/lld/lld_print_str.o     \
					src/lld/lld_read.o          \
					src/lld/lld_sort.o          \
					src/lld/lld_write.o         \
                                                \
                                                \
                                                \
					src/hw/int_handler.o        \

LINKER		=		builder/linker.ld

NAME		=		OS_S_kernel

INC			= 		-Iinclude -I../bootloader/inc

CFLAGS		=		$(INC) -Wall -Wextra 				\
					-fno-stack-protector -nostdinc		\
					-ffreestanding -fpie -nostdlib		\
					-mno-red-zone

LDFLAGS		=		-pie

all: font $(NAME)

$(NAME):$(OBJ)
	ld $(LINKER) $(LDFLAGS) -o $(NAME) $(OBJ)

font:
	make -C ./src/font/
	-./src/font/bdf2c -C ./src/font/font.h
	-./src/font/bdf2c -b < ./src/font/u_vga16.bdf > ./src/font/font.c

debug:	CFLAGS += -g
debug:	all

clean:
	find . -name "*~" -delete -o -name "#*#" -delete
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)
	rm -f ./src/font/font.c
	rm -f ./src/font/font.h
	rm -f ./src/font/bdf2c

re:	fclean all

install:all
	sudo mkdir -p /boot/EFI/OS_S
	sudo cp $(NAME) /boot/EFI/OS_S/

uninstall:
	sudo rm -f /boot/EFI/OS_S/$(NAME)
