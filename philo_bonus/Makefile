CC=gcc
CFLAGS=-Wall -Werror -Wextra -g#-fsanitize=thread#address
SRCS=main.c start.c time.c utils.c routine.c init.c watch.c msg.c handle_one.c
OBJS=$(SRCS:.c=.o)
DEPS=Makefile philo.h
NAME=philo_bonus

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

.PHONY: all clean fclean re philo_bonus
