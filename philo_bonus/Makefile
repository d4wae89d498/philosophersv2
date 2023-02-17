CC=gcc
CFLAGS=-Wall -Werror -Wextra -g #-fsanitize=address#-fsanitize=thread#address
SRCS=main.c time.c utils.c routine.c init.c watch.c msg.c
OBJS=$(SRCS:.c=.o)
DEPS=Makefile philo.h
NAME=philo_bonus

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
