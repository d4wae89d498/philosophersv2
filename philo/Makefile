CC=gcc
CFLAGS=-Wall -Werror -Wextra -g # -fsanitize=thread #address
SRCS=main.c philo.c routine.c time.c utils.c msg.c
OBJS=$(SRCS:.c=.o)
DEPS=Makefile philo.h
NAME=philo

all: 		$(NAME)

.SUFFIXES:

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

norm:
	python3 -m norminette

re: fclean all

.PHONY: all clean fclean re philo
