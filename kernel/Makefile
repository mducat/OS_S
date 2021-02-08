
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
                                                \
					src/screen/screen.o         \
					src/screen/data.o           \
                                                \
                                                \
                                                \
					src/hw/int_handler.o        \

LINKER		=		builder/linker.ld

NAME		=		OS_S_kernel

INC			= 		-Iinc -I../bootloader/inc -I../libc/inc
LIB			=		-L../libc -lc

CFLAGS		=		$(INC) -Wall -Wextra 				\
					-fno-stack-protector -nostdinc		\
					-ffreestanding -fpie -nostdlib		\
					-mno-red-zone

LDFLAGS		=		-pie

all:	$(NAME)

$(NAME):$(OBJ)
	ld $(LINKER) $(LDFLAGS) -o $(NAME) $(OBJ) $(LIB)

debug:	CFLAGS += -g
debug:	all

clean:
	find . -name "*~" -delete -o -name "#*#" -delete
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

install:all
	sudo mkdir -p /boot/EFI/OS_S
	sudo cp $(NAME) /boot/EFI/OS_S/

uninstall:
	sudo rm -f /boot/EFI/OS_S/$(NAME)
