NAME = philo
SRC = philo.c routine.c utils.c
OBJ = $(SRC:.c=.o)
FLAGS = -Wall -Wextra -Werror 

all: $(NAME)

$(NAME): $(OBJ) philo.h
	cc $(FLAGS) $(OBJ) -o $(NAME) 

%.o: %.c philo.h
	cc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all