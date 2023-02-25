/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 17:18:05 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 11:13:25 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	dmtx(pthread_mutex_t *mtx)
{
	(void) mtx;
	pthread_mutex_destroy(mtx);
	return (1);
}

static int	start_watcher(t_args args, pthread_t *philos,
		t_philo_ctx *philos_ctx)
{
	static pthread_t		watcher;
	static t_watcher_args	watcher_args;

	watcher_args = (t_watcher_args){.args = args, .philos = philos,
		.philos_ctx = philos_ctx, .dead = 0};
	if (pthread_create(&watcher, 0, &watch_philos, &watcher_args))
		return (!!ft_eputs("Error: pthread_create.\n"));
	if (pthread_join(watcher, 0))
		return (!!ft_eputs("Error: pthread_join.\n"));
	return (0);
}

// TODO :: what if ini_philo_ctx fail
static int	start(t_args args)
{
	static t_philo_ctx		philos_ctx[MAX_THREADS];
	static t_mtx			table[MAX_THREADS];
	static pthread_t		philos[MAX_THREADS];
	pthread_mutex_t			console;

	args.start = current_time(0);
	if (pthread_mutex_init(&console, 0))
		return (!!ft_eputs("Error: pthread_mutex_init.\n"));
	if (init_table(table, args.number_of_philos)
		|| init_philos_ctx(args, table, philos_ctx, &console))
		return (!!dmtx(&console));
	if (init_philos(args.number_of_philos, philos_ctx, philos))
		return (!!(dmtx(&console)
				+ destroy_philos_ctx(philos_ctx, args.number_of_philos) + 1));
	if (start_watcher(args, philos, philos_ctx))
		return (!!(dmtx(&console)
				+ destroy_philos_ctx(philos_ctx, args.number_of_philos) + 1));
	while ((args.number_of_philos)--)
		pthread_join(philos[args.number_of_philos], 0);
	return (!!(dmtx(&console))
		+ destroy_philos_ctx(philos_ctx, args.number_of_philos) + 1);
}

int	main(int ac, char **av)
{
	static t_args			args;

	if (ac != 5 && ac != 6)
		return (!!ft_eputs("Error: invalid arguments.\n"));
	args.number_of_philos = ft_atol(av[1]);
	args.time_to_die = ft_atol(av[2]);
	args.time_to_eat = ft_atol(av[3]);
	args.time_to_sleep = ft_atol(av[4]);
	args.number_of_meals = -1;
	if (args.number_of_philos < 1 || args.time_to_die < 0
		|| args.time_to_eat < 0 || args.time_to_sleep < 0
		|| args.number_of_philos > MAX_THREADS)
		return (!!ft_eputs("Error: invalid arguments.\n"));
	if (ac == 6)
	{
		args.number_of_meals = ft_atol(av[5]);
		if (args.number_of_meals < 0)
			return (!!ft_eputs("Error: invalid arguments.\n"));
		else if (args.number_of_meals == 0)
			return (0);
	}
	return (start(args));
}
