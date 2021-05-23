
OBJ		=	src/write.o  \
			src/memory.o  \
			src/fs.o  \
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

NAME	=	liboss.a

INC		= 	-Iinc -I../kernel/include

CFLAGS	=	$(INC) -Wall -Wextra 				\
			-ffreestanding -nostdinc -nostdlib	\
			-D_OSS_SOURCE

all: $(NAME)

$(NAME):$(OBJ)
	ar rcv $(NAME) $(OBJ)

debug:	CFLAGS += -g
debug:	fclean all

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
