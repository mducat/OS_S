
OBJ		=	src/write.o  \
			src/memory.o  \
			src/fs.o  \
			src/draw.o  \
			src/screen.o \
			src/printf/conf.o  \
			src/printf/is_num.o  \
			src/printf/my_printf.o  \
			src/printf/my_put_unsigned_nbr.o  \
			src/printf/my_putnbr_base.o  \
			src/printf/parser1.o  \
			src/printf/parser2.o  \
			src/printf/pf_put_nbr.o  \
			src/printf/printers1.o  \
			src/printf/printers2.o  \
			src/printf/printers3.o  \
			src/printf/printers4.o  \
			src/printf/put_double.o  \
			src/printf/utils.o  \
			src/lld/lld_db.o  \
			src/lld/lld_free.o  \
			src/lld/lld_init.o  \
			src/lld/lld_insert_node.o  \
			src/lld/lld_insert.o  \
			src/lld/lld_len_db.o  \
			src/lld/lld_len.o  \
			src/lld/lld_lld_to_tab.o  \
			src/lld/lld_pop_node.o  \
			src/lld/lld_pop.o  \
			src/lld/lld_read.o  \
			src/lld/lld_sort.o  \
			src/lld/lld_write.o  \
			src/utils.o  \
			src/ucp/is_.o  \
			src/ucp/is_made.o  \
			src/ucp/ucp_tab.o  \
			src/ucp/ucp.o  \

NAME	=	liboss.a

INC		= 	-Iinc -I../kernel/include

CFLAGS	=	$(INC) -Wall -Wextra 				\
			-ffreestanding -nostdinc -nostdlib	\
			-D_OSS_SOURCE -fpie

CC		=	x86_64-elf-gcc
AR		=	x86_64-elf-ar

all: $(NAME)

$(NAME):$(OBJ)
	$(AR) rcv $(NAME) $(OBJ)

debug:	CFLAGS += -g
debug:	fclean all

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
