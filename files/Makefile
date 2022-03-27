
NAME		=	output

PROGRAMS	=	ls	\
				cat \
				help \
				aoss \
				omac \
				coss \
				export \
				imshow \
				world \

PLAIN		=   plain

IMAGES		=	images

all:
	mkdir -p $(NAME)
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir --no-print-directory ; \
		sed -i '1s/^/OSS/' $$dir/$$dir; \
		mv $$dir/$$dir $(NAME)/ ; \
	done
	cp $(PLAIN)/* $(NAME)/
	cp $(IMAGES)/* $(NAME)/

clean:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir clean --no-print-directory ; \
	done

debug:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir debug --no-print-directory ; \
	done

fclean:
	for dir in $(PROGRAMS) ; do \
		$(MAKE) -C $$dir fclean --no-print-directory ; \
	done
	rm -rf $(NAME)

re:	fclean all

