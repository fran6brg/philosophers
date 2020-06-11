NAME = philo_one

FLAGS = -Wall -Wextra -Werror

SRCS = main.c utils.c init.c actions.c

OBJS = ${SRCS:.c=.o}

DEBUG = -g -fsanitize=address

# ifndef VERBOSE
# .SILENT:
# endif

all: $(NAME)

.c.o:
	clang $(FLAGS) -c $< -o $@

$(NAME): $(OBJS)
	clang $(FLAGS) $(SRCS) -o $(NAME) -lpthread

x: all
	@./$(NAME) 4 410 200 200

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
