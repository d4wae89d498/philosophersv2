#CC=/usr/local/Cellar/gcc/12.2.0/bin/gcc-12
CC=gcc
CFLAGS=-Wall -Werror -Wextra -g -fsanitize=thread
SRCS=destroy.c main.c watcher.c routine.c time.c puts.c msg.c init.c utils.c
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

norm:
	python3 -m norminette

re: fclean all

.PHONY: all clean fclean re
