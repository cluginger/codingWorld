NAME = littlearrow
NCURSES = $(shell ncursesw5-config --cflags --libs)

all: $(NAME)

$(NAME): $(NAME).c utils.c
		gcc -Wall -Werror -std=gnu11 -pthread -o $(NAME) $(NAME).c utils.c $(NCURSES)
		
run:
	./$(NAME)
		
clean:
	rm -rf $(NAME) $(NAME)

