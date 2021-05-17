
OBJ		=	src/write.o  \

NAME	=	liboss.a

INC		= 	-Iinc -I../../include

CFLAGS	=	$(INC) -Wall -Wextra 				\
			-nostdinc -ffreestanding -nostdlib	\
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
