CC=gcc
CFLAGS=-Wall -Werror -Wextra -g# -fsanitize=thread #address
SRCS=destroy.c main.c watcher.c routine.c time.c utils.c msg.c init.c
OBJS=$(SRCS:.c=.o)
DEPS=Makefile philo.h
NAME=philo

all: 		$(NAME)

.SUFFIXES:

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) -lpthread -o $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re philo
