#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef LOG
# define LOG 1
#endif

typedef enum e_state
{
	penser,
	manger,
	dormir 
}	t_state;

typedef struct	s_philo 
{
	unsigned int		id;
	t_state				state;	
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	unsigned long		last_update;	
}	t_philo_ctx;

typedef struct	s_args
{
	long	number_of_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_args;

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

t_philo_ctx	*init_philos_ctx(long number_of_philos, pthread_mutex_t	*table)
{
	t_philo_ctx	*philos_ctx;
	long		i;

	philos_ctx = malloc(number_of_philos * sizeof(t_philo_ctx));
	i = 0;
	while (i < number_of_philos)
	{
		philos_ctx[i].id = i + 1;
		philos_ctx[i].state = penser;
		philos_ctx[i].last_update = 0;
		
		if (i == number_of_philos - 1)
		{
			philos_ctx[i].left_fork = table + number_of_philos - 1;
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

int main(int ac, char **av)
{
	t_args			args;
	t_philo_ctx		*philos_ctx;
	
	pthread_mutex_t	*table;
	pthread_t		*philos;

	if (ac != 5 && ac != 6)
		exit(0);
	args.number_of_philos = atol(av[1]);
	args.time_to_die = atol(av[2]);
	args.time_to_eat = atol(av[3]);
	args.time_to_sleep = atol(av[4]);
	if (ac == 4)
		args.number_of_meals = -1;
	else
		args.number_of_meals = atol(av[5]);
	log_args(args);
	table = init_table(args.number_of_philos);
	philos_ctx = init_philos_ctx(args.number_of_philos, table);	
	return (0);
}
