# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: francisberger <francisberger@student.42    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/01/07 17:41:37 by fberger           #+#    #+#              #
#    Updated: 2020/06/22 22:05:23 by francisberg      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			= 	clang

FLAGS 		= 	-Wall -Wextra -Werror

SRCS_1		=	philo_one/actions.c \
				philo_one/init.c \
				philo_one/main.c \
				philo_one/logs.c \
				philo_one/utils.c \
				philo_one/end.c
OBJS_1		= 	$(SRCS_1:.c=.o)
EXEC_1		=	philo_one/philo_one

SRCS_2		=	philo_two/actions.c \
				philo_two/init.c \
				philo_two/main.c \
				philo_two/logs.c \
				philo_two/utils.c \
				philo_two/end.c
OBJS_2		= 	$(SRCS_2:.c=.o)
EXEC_2		=	philo_two/philo_two

SRCS_3		=	philo_three/actions.c \
				philo_three/init.c \
				philo_three/main.c \
				philo_three/logs.c \
				philo_three/utils.c \
				philo_three/end.c
OBJS_3		= 	$(SRCS_3:.c=.o)
EXEC_3		=	philo_three/philo_three

all: 1 2 3

1:	
	${CC} ${FLAGS} $(SRCS_1) -o $(EXEC_1) -lpthread

2:
	${CC} ${FLAGS} $(SRCS_2) -o $(EXEC_2) -lpthread

3:
	${CC} ${FLAGS} $(SRCS_3) -o $(EXEC_3) -lpthread

# tests 

A 			=	5 800 200 200
B 			=	5 800 200 200 7
C 			=	4 410 200 200
D 			=	4 310 200 100
E 			=	2 310 200 100

# 1
1A:
	@./$(EXEC_1) $(A)
1B:
	@./$(EXEC_1) $(B)
1C:
	@./$(EXEC_1) $(C)
1D:
	@./$(EXEC_1) $(D)
1E:
	@./$(EXEC_1) $(E)

# 2
2A:
	@./$(EXEC_2) $(A)
2B:
	@./$(EXEC_2) $(B)
2C:
	@./$(EXEC_2) $(C)
2D:
	@./$(EXEC_2) $(D)
2E:
	@./$(EXEC_2) $(E)

# 3
3A:
	@./$(EXEC_3) $(A)
3B:
	@./$(EXEC_3) $(B)
3C:
	@./$(EXEC_3) $(C)
3D:
	@./$(EXEC_3) $(D)
3E:
	@./$(EXEC_3) $(E)

clean:
	@/bin/rm -f $(OBJS_1)
	@/bin/rm -f $(OBJS_2)
	@/bin/rm -f $(OBJS_3)

fclean: clean
	@/bin/rm -f $(EXEC_1)
	@/bin/rm -f $(EXEC_2)
	@/bin/rm -f $(EXEC_3)

re: fclean all
