
OBJ		= src/memory.o  \
		  src/string.o  \
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

NAME	= libc.a

INC		= -Iinc

CFLAGS	= -Wall -Wextra -nostdinc \
		  -ffreestanding -nostdlib \
		  $(INC)

all:	$(NAME)

$(NAME):$(OBJ)
	ar rcsv $(NAME) $(OBJ)

debug:	CFLAGS += -g
debug:	all

clean:
	find . -name "*~" -delete -o -name "#*#" -delete
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re:	fclean all
