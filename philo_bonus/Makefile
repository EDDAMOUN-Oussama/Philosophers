NAME = philo_bonus
SRC = philo_bonus.c routine_bonus.c utils_bonus.c utils2_bonus.c
OBJ = $(SRC:.c=.o)
FLAGS = -Wall -Wextra -Werror
all: $(NAME)

$(NAME): $(OBJ) philo_bonus.h
	cc $(FLAGS) $(OBJ) -o $(NAME) 

%.o: %.c philo_bonus.h
	cc $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all