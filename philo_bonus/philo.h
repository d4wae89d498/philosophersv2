#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <semaphore.h>
# include <sys/time.h>
# ifndef LOG
#  define LOG 0
# endif
# ifndef MAX_PROCESS
#  define MAX_PROCESS 84000
# endif

typedef struct	s_args
{
	long	number_of_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_args;
typedef enum e_state
{
	penser,
	manger,
	dormir,
	plus_faim,
}	t_state;


unsigned long	current_time();
void			ft_sleep(unsigned long time);
#endif
