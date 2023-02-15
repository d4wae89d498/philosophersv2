#include "philo.h"

static long	ft_atol(char *s)
{
	long	n;

	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		n *= 10;
		n += *s++ - '0';
	}
	if (*s)
		return (-1);
	return (n);
}

/*
void	log_args(t_args args)
{
	(void) args;
#if LOG
	printf("time_to_philos: %li\n", args.number_of_philos);
	printf("time_to_die: %li\n", args.time_to_die);
	printf("time_to_eat: %li\n", args.time_to_eat);
	printf("time_to_sleep: %li\n", args.time_to_sleep);
	printf("number_of_meals: %li\n", args.number_of_meals);
#endif
}
*/

static int		try_destroy(pthread_mutex_t *mtx)
{
	if (pthread_mutex_destroy(mtx))
		return (!!printf("Error: pthread_mutex_destroy.\n"));
	return (0);
}

static int		start(t_args args, pthread_mutex_t *console, 
		pthread_t *watcher, t_watcher_args *watcher_args)
{
	static t_philo_ctx		philos_ctx[MAX_THREADS];
	static pthread_mutex_t	table[MAX_THREADS];
	static pthread_t		philos[MAX_THREADS];
	int						r;

	if (args.number_of_meals == 0)
		return (try_destroy(console));
	r = 0;
	if (init_table(table, args.number_of_philos))
		return (!!printf("Error: pthread_mutex_init.\n"));
	init_philos_ctx(args, console, table, philos_ctx);	
	if (init_philos(args.number_of_philos, philos_ctx, philos))
		r = !!printf("Error: pthread_create.\n");
	else
	{
		watcher_args->args = args;
		watcher_args->philos_ctx = philos_ctx;
		watcher_args->philos = philos;
		pthread_create(watcher, 0, &watch_philos, watcher_args);
		pthread_join(*watcher, 0);
	}
	while ((args.number_of_philos)--)
		pthread_join(philos[args.number_of_philos], 0);
	return (!!(r + try_destroy(console)));
}

int main(int ac, char **av)
{
	static t_args			args;
	static pthread_mutex_t	console;
	static pthread_t		watcher;
	static t_watcher_args	watcher_args;

	if (ac != 5 && ac != 6)
		return (!!printf("Error: invalid arguments.\n"));
	args.number_of_philos = ft_atol(av[1]);
	args.time_to_die = ft_atol(av[2]);
	args.time_to_eat = ft_atol(av[3]);
	args.time_to_sleep = ft_atol(av[4]);
	args.number_of_meals = -1;
	if (ac == 6)
	{
		args.number_of_meals = ft_atol(av[5]);
		if (args.number_of_meals < 0)
			return (!!printf("Error: invalid arguments.\n"));
	}
	if (args.number_of_philos < 0 || args.time_to_die < 0
			|| args.time_to_eat < 0 || args.time_to_sleep < 0)
			return (!!printf("Error: invalid arguments.\n"));
	if (pthread_mutex_init(&console, 0))
		return (!!printf("Error: pthread_mutex_init.\n"));
	return (start(args, &console, &watcher, &watcher_args));
}
