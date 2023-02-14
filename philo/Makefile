CC=gcc
CFLAGS=-Wall -Werror -Wextra -g
SRCS=main.c philo.c routine.c time.c
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

re: fclean all

.PHONY: all clean fclean re philo
