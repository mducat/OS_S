
NAME		=	files.h

PROGRAMS	=	ls	\

PLAIN		=   $(shell ls plain/*)

LIB			=	oss_lib

MKHEADER	=	mkhdr.py

all:
	$(MAKE) -C $(LIB) --no-print-directory
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir --no-print-directory ; \
	done

	prog=() ; \
	for i in $(PROGRAMS) ; do \
		prog+=($$i/$$i) ; \
	done ; \
	python $(MKHEADER) $$prog and $(PLAIN) > $(NAME)

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
