
OBJ =	lld/lld_db.o  \
			lld/lld_free.o  \
			lld/lld_init.o  \
			lld/lld_insert_node.o  \
			lld/lld_insert.o  \
			lld/lld_len_db.o  \
			lld/lld_len.o  \
			lld/lld_lld_to_tab.o  \
			lld/lld_pop_node.o  \
			lld/lld_pop.o  \
			lld/lld_read.o  \
			lld/lld_sort.o  \
			lld/lld_write.o  \
			ucp/is_.o  \
			ucp/is_made.o  \
			ucp/ucp_tab.o  \
			ucp/ucp.o  \
			utils.o  \

NAME	=	liboss.a

INC		= 	-I.

CFLAGS	=	$(INC) -Wall -Wextra

CC		=	gcc
AR		=	ar

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

