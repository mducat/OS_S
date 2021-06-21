
NAME		=	files_raw.h

PROGRAMS	=	ls	\
				cat \
				help \
				aoss \

PLAIN		=   $(shell ls plain/*)

MKHEADER	=	mkhdr.py

all:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir --no-print-directory ; \
	done

	python3 $(MKHEADER) $(PROGRAMS) and $(PLAIN) > $(NAME)

clean:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir clean --no-print-directory ; \
	done

debug:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir debug --no-print-directory ; \
	done

	python3 $(MKHEADER) $(PROGRAMS) and $(PLAIN) > $(NAME)

fclean:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir fclean --no-print-directory ; \
	done
	rm -f $(NAME)

re:	fclean all

