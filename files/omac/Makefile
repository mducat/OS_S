
OBJ		=	src/omac.o   \

NAME	=	omac

INC		= 	-I../../oss_lib/inc -I../../kernel/include

LIBS	=	-L../../oss_lib -loss

CFLAGS	=	$(INC) -Wall -Wextra 				\
			-nostdinc -ffreestanding -nostdlib -fpie \
			-Wl,-e"main"

CC		=	x86_64-elf-gcc

all: $(NAME)

$(NAME):$(OBJ)
	gcc $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS)

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

debug: CFLAGS += -g
debug: fclean all
