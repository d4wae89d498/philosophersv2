/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 17:18:05 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 06:49:49 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	dmtx(pthread_mutex_t *mtx)
{
	pthread_mutex_destroy(mtx);
	return (0);
}

static int	manage_mutexes(int destroy, t_mutexes **ptr)
{
	static t_mutexes	m;

	if (!destroy)
	{
		*ptr = &m;
		if (pthread_mutex_init(&(m.console), 0))
			return (1);
		if (pthread_mutex_init(&(m.dead_console), 0))
			return (!!(dmtx(&(m.console) + 1)));
		if (pthread_mutex_init(&(m.gdead_mtx), 0))
			return (!!(dmtx(&(m.console)) + dmtx(&(m.dead_console) + 1)));
		return (0);
	}
	else
		return (!!(dmtx(&(m.console)) + dmtx(&(m.console))
				+ dmtx(&(m.dead_console))));
}

static int	start_watcher(t_args args, pthread_t *philos,
		t_philo_ctx *philos_ctx)
{
	static pthread_t		watcher;
	static t_watcher_args	watcher_args;

	watcher_args = (t_watcher_args){.args = args, .philos = philos,
		.philos_ctx = philos_ctx};
	if (pthread_create(&watcher, 0, &watch_philos, &watcher_args))
		return (!!ft_puts("Error: pthread_create.\n"));
	else if (pthread_join(watcher, 0))
		return (!!ft_puts("Error: pthread_join.\n"));
	return (0);
}

static int	start(t_args args)
{
	static t_philo_ctx		philos_ctx[MAX_THREADS];
	static pthread_mutex_t	table[MAX_THREADS];
	static pthread_t		philos[MAX_THREADS];
	int						r;
	t_mutexes				*m;

	if (manage_mutexes(0, &m))
		return (!!ft_puts("Error: pthread_mutex_init.\n"));
	if (init_table(table, args.number_of_philos))
		return (!!ft_puts("Error: pthread_mutex_init.\n")
			+ manage_mutexes(1, 0));
	init_philos_ctx(args, table, philos_ctx, m);
	r = 0;
	if (init_philos(args.number_of_philos, philos_ctx, philos))
		r = !!ft_puts("Error: pthread_create.\n");
	else
		r = start_watcher(args, philos, philos_ctx);
	while ((args.number_of_philos)--)
		pthread_join(philos[args.number_of_philos], 0);
	return (!!(r + manage_mutexes(1, 0)));
}

int	main(int ac, char **av)
{
	static t_args			args;

	if (ac != 5 && ac != 6)
		return (!!ft_puts("Error: invalid arguments.\n"));
	args.number_of_philos = ft_atol(av[1]);
	args.time_to_die = ft_atol(av[2]);
	args.time_to_eat = ft_atol(av[3]);
	args.time_to_sleep = ft_atol(av[4]);
	args.number_of_meals = -1;
	if (ac == 6)
	{
		args.number_of_meals = ft_atol(av[5]);
		if (args.number_of_meals < 0)
			return (!!ft_puts("Error: invalid arguments.\n"));
		else if (args.number_of_meals == 0)
			return (0);
	}
	if (args.number_of_philos < 0 || args.time_to_die < 0
		|| args.time_to_eat < 0 || args.time_to_sleep < 0)
		return (!!ft_puts("Error: invalid arguments.\n"));
	return (start(args));
}
