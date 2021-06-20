
OBJ		=	src/main.c   \

OBJ_COMPILATOR = compilator.c instructions.c utils.c

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

peak_compiler:
	rm -f a.out
	gcc -g  $(OBJ_COMPILATOR) lld/*.c ucp/*.c
	objdump -D -Mintel,x86-64 -b binary -m i386 a.out


run:
	rm -f a.out
	gcc -g $(OBJ_COMPILATOR) lld/*.c ucp/*.c
	-./a.out main.coss
	####################################################################################################
	-objdump -D -Mintel,x86-64 -b binary -m i386 main.oss

runO4:
	rm -f a.out
	gcc $(OBJ_COMPILATOR) lld/*.c ucp/*.c -O4
	./a.out main.coss


gdb:
	rm -f a.out
	gcc -g $(OBJ_COMPILATOR) lld/*.c ucp/*.c
	gdb ./a.out main.coss


valgrind:
	clear
	rm -f a.out
	gcc -g $(OBJ_COMPILATOR) lld/*.c ucp/*.c
	-valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./a.out main.coss | cat -e
	####################################################################################################
	-objdump -D -Mintel,x86-64 -b binary -m i386 main.oss

clean:
	rm -f  $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
