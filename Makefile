CC = gcc
CFLAGS = -Wall -Wextra -Werror -lpthread
SRC = main.c parsing.c init.c dongles.c action.c time.c
OBJ = $(SRC:.c=.o)
NAME = ./codexion

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
