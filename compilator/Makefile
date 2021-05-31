
OBJ		=	src/main.c   \

NAME	=	ls

INC		= 	-I../../oss_lib/inc -I../../kernel/include

LIBS	=	-L../../oss_lib -loss

CFLAGS	=	$(INC) -Wall -Wextra 				\
			-nostdinc -ffreestanding -nostdlib	\
			-Wl,--oformat=binary 

#objdump -D -Mintel,x86-64 -b binary -m i386 a.out

all:

peak:
	rm -f a.out
	gcc main.c $(CFLAGS)
	objdump -D -Mintel,x86-64 -b binary -m i386 a.out

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
