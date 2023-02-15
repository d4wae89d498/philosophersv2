#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# ifndef LOG
#  define LOG 0
# endif
# ifndef MAX_THREADS
#  define MAX_THREADS 42000
# endif
# include "stdlib.h"
# include "unistd.h"
# include "sys/time.h"

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
typedef struct	s_philo_ctx
{
	unsigned int		id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*console;
	pthread_mutex_t		*dead_console;
	t_args				args;
	volatile t_state
		state;	
	volatile unsigned long
		last_eat_time;
	volatile long 		meals;
	volatile int		dead;
	volatile int		*gdead;
}	t_philo_ctx;
typedef struct s_watcher_args
{
	pthread_t	*philos;
	t_philo_ctx	*philos_ctx;
	t_args		args;
}	t_watcher_args;

int		try_eat(t_philo_ctx *ctx);
void	*philo_routine(void *data);
void	msg(t_philo_ctx *ctx, char *msg);
int		init_table(pthread_mutex_t *table, long number_of_philos);
void	init_philos_ctx(t_args args, pthread_mutex_t *console,
			pthread_mutex_t	*table, t_philo_ctx *philos_ctx, pthread_mutex_t *dead_console, volatile int *gdead);
int		init_philos(long number_of_philos, t_philo_ctx *philos_ctx,
			pthread_t *philos);
void	*watch_philos(void *data);
unsigned long
		current_time();
void	ft_sleep(unsigned long time);
#endif
