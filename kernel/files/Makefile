
NAME		=	files_raw.h

PROGRAMS	=	ls	\
				cat \
				help \

PLAIN		=   $(shell ls plain/*)

LIB			=	oss_lib

MKHEADER	=	mkhdr.py

all:
	$(MAKE) -C $(LIB) --no-print-directory

	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir re --no-print-directory ; \
	done

	python $(MKHEADER) $(PROGRAMS) and $(PLAIN) > $(NAME)

clean:
	$(MAKE) -C $(LIB) clean --no-print-directory

	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir clean --no-print-directory ; \
	done

fclean:
	$(MAKE) -C $(LIB) fclean --no-print-directory

	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir fclean --no-print-directory ; \
	done
	rm -f $(NAME)

re:	fclean all
