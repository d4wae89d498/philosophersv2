#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef LOG
# define LOG 0
#endif
#include "stdlib.h"
#include "unistd.h"
#include "sys/time.h"

unsigned long	current_time()
{
	struct timeval 	time;
	static unsigned long	start;

	gettimeofday(&time, 0);
	if (start == 0)
		start = ((time.tv_sec * 1000000 + time.tv_usec));
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	ft_sleep(unsigned long time)
{
	unsigned long	start;

	time *= 1000;
	start = current_time();


	usleep(time * 0.80);
	while (current_time() < start + time)
	{
		usleep(10);
	}
}

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
	t_args				args;
	volatile t_state
						state;	
	volatile unsigned long
						last_eat_time;
	volatile long 		meals;
	volatile int		dead;
}	t_philo_ctx;

typedef struct s_watcher_args
{
	pthread_t	*philos;
	t_philo_ctx	*philos_ctx;
	t_args		args;
}	t_watcher_args;

void	log_args(t_args args)
{
#if LOG
	printf("time_to_philos: %li\n", args.number_of_philos);
	printf("time_to_die: %li\n", args.time_to_die);
	printf("time_to_eat: %li\n", args.time_to_eat);
	printf("time_to_sleep: %li\n", args.time_to_sleep);
	printf("number_of_meals: %li\n", args.number_of_meals);
#endif
}



void	msg(t_philo_ctx *ctx, char *msg)
{

	pthread_mutex_lock(ctx->console);
	printf("%lu philo %u %s\n", (unsigned long)(current_time() * 0.001), ctx->id, msg);
	pthread_mutex_unlock(ctx->console);
}

pthread_mutex_t	*init_table(long number_of_philos)
{
	pthread_mutex_t		*table;
	long				i;

	table = malloc(number_of_philos * sizeof(pthread_mutex_t));
	i = 0;
	while (i < number_of_philos)
		pthread_mutex_init(table + i++, 0);	
	return (table);
}

t_philo_ctx	*init_philos_ctx(t_args args, pthread_mutex_t	*table, pthread_mutex_t *console)
{
	t_philo_ctx	*philos_ctx;
	long		i;

	philos_ctx = malloc(args.number_of_philos * sizeof(t_philo_ctx));
	i = 0;
	while (i < args.number_of_philos)
	{
		philos_ctx[i].id = i + 1;
		philos_ctx[i].state = penser;
		philos_ctx[i].last_eat_time = 0;
		philos_ctx[i].console = console;
		philos_ctx[i].args = args;
		philos_ctx[i].meals = 0;
		philos_ctx[i].dead = 0;

		if (i == args.number_of_philos - 1)
		{
			philos_ctx[i].left_fork = table + args.number_of_philos - 1;
			philos_ctx[i].right_fork = table + 0;
		}
		else
		{
			philos_ctx[i].left_fork = table + i;
			philos_ctx[i].right_fork = table + i + 1;
		}

		i += 1;
	}
	return (philos_ctx);
}

void	*philo_routine(void *data)
{
	t_philo_ctx	*ctx;

	ctx = data;
	if (ctx->id % 2)
		ft_sleep(1);
	while (ctx->meals < ctx->args.number_of_meals
			|| (ctx->args.number_of_meals < 0))
	{
		if (ctx->dead)
			return (0);
		pthread_mutex_lock(ctx->left_fork);
		if (ctx->dead)
			return (0);
		msg(ctx, "has taken a fork");
		pthread_mutex_lock(ctx->right_fork);
		if (ctx->dead)
			return (0);
		ctx->state = manger; 
		ctx->last_eat_time = current_time();
		ctx->meals += 1;
		msg(ctx, "has taken a fork");
		msg(ctx, "is eating");
		ft_sleep(ctx->args.time_to_eat);
		if (ctx->dead)
			return (0);
		pthread_mutex_unlock(ctx->right_fork);
		pthread_mutex_unlock(ctx->left_fork);
		ctx->state = dormir;
		msg(ctx, "is sleeping");
		ft_sleep(ctx->args.time_to_sleep);
		if (ctx->dead)
			return (0);
		ctx->state = penser;	
		msg(ctx, "is thinking");
	}
	ctx->state = plus_faim;
	return (0);
}

pthread_t	*init_philos(long number_of_philos, t_philo_ctx *philos_ctx)
{
	pthread_t	*philos;
	long		i;

	philos = malloc(number_of_philos * sizeof(pthread_t));
	i = 0;
	while (i < number_of_philos)
	{
		pthread_create(philos + i, 0, &philo_routine, philos_ctx + i);
		i += 1;
	}

	return (philos);
}

void	*watch_philos(void *data)
{
	t_watcher_args	*watcher_args = data;
	long	i;
	long	y;
	int		sexit;


	sexit = 0;
	while (!sexit)
	{
		y = 0;
		i = 0;
		while (!sexit && (i < watcher_args->args.number_of_philos))
		{
			if (watcher_args->philos_ctx[i].state == plus_faim)
			{
				y += 1;
			}
			else
			{
				if (current_time() - watcher_args->philos_ctx[i].last_eat_time  > watcher_args->args.time_to_die * 1000 
						&& watcher_args->philos_ctx[i].state != manger)
				{
					msg(watcher_args->philos_ctx + i, "died");
					sexit = 1;
				}
			}
			i += 1;
		}
		if (y == watcher_args->args.number_of_philos)
			return (0);
	}
	
	i = 0;
	while (i < watcher_args->args.number_of_philos)
	{
		
		watcher_args->philos_ctx[i].dead = 1;
		i += 1;
	}
	return (0);
}

int main(int ac, char **av)
{
	t_args			args;
	t_philo_ctx		*philos_ctx;

	pthread_mutex_t	*table;
	pthread_t		*philos;
	pthread_t		watcher;

	pthread_mutex_t	console;

	t_watcher_args	watcher_args;

	if (ac != 5 && ac != 6)
		exit(0);
	args.number_of_philos = atol(av[1]);
	args.time_to_die = atol(av[2]);
	args.time_to_eat = atol(av[3]);
	args.time_to_sleep = atol(av[4]);
	if (ac == 5)
		args.number_of_meals = -1;
	else
		args.number_of_meals = atol(av[5]);

	pthread_mutex_init(&console, 0);

	log_args(args);
	table = init_table(args.number_of_philos);
	philos_ctx = init_philos_ctx(args, table, &console);	

#if LOG
	printf("\n");
#endif

	philos = init_philos(args.number_of_philos, philos_ctx);


	watcher_args.args = args;
	watcher_args.philos_ctx = philos_ctx;
	watcher_args.philos = philos;

	pthread_create(&watcher, 0, &watch_philos, &watcher_args);
	pthread_join(watcher, 0);

	free(philos);
	free(philos_ctx);
	free(table);

	return (0);
}
