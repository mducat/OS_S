
OBJ		=	src/coss.o   \
			src/instructions.o   \

SRC		=	$(OBJ:.o=.c)

NAME	=	coss

INC_OSS		= 	-I../../oss_lib/inc -I../../kernel/include

LIBS_OSS	=	-L../../oss_lib -loss


INC		= 	-I../../testlib

LIBS	=	-L../../testlib -loss

CFLAGS	=	$(INC_OSS) -Wall -Wextra 				\
			-nostdinc -ffreestanding -nostdlib -fpie \
			-Wl,-e"main"

CC		=	x86_64-elf-gcc

all: $(NAME)

$(NAME):$(OBJ)
	gcc $(CFLAGS) -o $(NAME) $(OBJ) $(LIBS_OSS)

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all

debug: CFLAGS += -g
debug: fclean all

valgrind: fclean
	clear
	gcc -g $(SRC) $(INC) $(LIBS) -o $(NAME) -Wall -Wextra
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME) main.coss | cat -e
	###################################################################################################
	cat main.aoss
	rm main.aoss
