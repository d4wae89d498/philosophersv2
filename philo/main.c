#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef LOG
# define LOG 1
#endif

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
	dormir 
}	t_state;

typedef struct	s_philo_ctx
{

	unsigned int		id;
	t_state				state;	
	long 				meals;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	unsigned long		last_update;
	pthread_mutex_t		*console;
	t_args				args;
}	t_philo_ctx;



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
		philos_ctx[i].last_update = 0;
		philos_ctx[i].console = console;
		philos_ctx[i].args = args;
		philos_ctx[i].meals = 0;

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
		ft_sleep(10);

	while (ctx->meals < ctx->args.number_of_meals
			|| (ctx->args.number_of_meals < 0))
	{
		pthread_mutex_lock(ctx->left_fork);
		msg(ctx, "has taken a fork");
		pthread_mutex_lock(ctx->right_fork);
		msg(ctx, "has taken a fork");
		ctx->state = manger; 
		msg(ctx, "is eating");
		ft_sleep(ctx->args.time_to_eat);
		pthread_mutex_unlock(ctx->right_fork);
		pthread_mutex_unlock(ctx->left_fork);


		msg(ctx, "is sleeping");
		ft_sleep(ctx->args.time_to_sleep);

		msg(ctx, "is thinking");
	}
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

void	*watch_philos(void *philos_ctx)
{
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


	pthread_create(&watcher, 0, &watch_philos, philos_ctx);

	// todo : join threads
	usleep(200000000);

	return (0);
}
