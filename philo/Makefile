NAME = philo
SRC = philo.c routine.c utils.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) philo.h
	cc -Wall -Wextra -Werror $(OBJ) -o $(NAME) 

%.o: %.c philo.h
	cc -Wall -Wextra -Werror -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all