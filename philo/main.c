/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 17:18:05 by mfaussur          #+#    #+#             */
/*   Updated: 2023/03/31 13:49:43 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	start(t_args args)
{
	static t_philo_ctx		philos_ctx[MAX_THREADS];
	static pthread_mutex_t	table[MAX_THREADS];
	static pthread_t		philos[MAX_THREADS];
	pthread_mutex_t			console;
	void					*exit_status;

	args.start = current_time(0);
	if (pthread_mutex_init(&console, 0))
		return (ft_eputs("Error: pthread_mutex_init.\n"));
	if (init_table(table, args.number_of_philos)
		|| init_philos_ctx(args, table, philos_ctx, &console))
		return (destroy_mutex(&console) + 1);
	if (init_philos(args.number_of_philos, philos_ctx, philos))
		return ((destroy_mutex(&console)
				+ destroy_philos_ctx(philos_ctx, args.number_of_philos) + 1));
	if (start_watcher(args, philos, philos_ctx))
		return ((destroy_mutex(&console)
				+ destroy_philos_ctx(philos_ctx, args.number_of_philos) + 1));
	while ((args.number_of_philos)--)
		pthread_join(philos[args.number_of_philos], &exit_status);
	return ((destroy_mutex(&console))
		+ destroy_philos_ctx(philos_ctx, args.number_of_philos));
}

static int	parse(t_args *args, int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (ft_eputs("Error: invalid arguments.\n"));
	args->number_of_philos = ft_atol(av[1]);
	args->time_to_die = ft_atol(av[2]);
	args->time_to_eat = ft_atol(av[3]);
	args->time_to_sleep = ft_atol(av[4]);
	args->number_of_meals = -1;
	if (args->number_of_philos < 1 || args->time_to_die < 0
		|| args->time_to_eat < 0 || args->time_to_sleep < 0
		|| args->number_of_philos > MAX_THREADS)
		return (ft_eputs("Error: invalid arguments.\n"));
	if (ac == 6)
	{
		args->number_of_meals = ft_atol(av[5]);
		if (args->number_of_meals < 0)
			return (ft_eputs("Error: invalid arguments.\n"));
		else if (args->number_of_meals == 0)
			return (1);
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_args	args;
	int		exit_code;

	exit_code = parse(&args, ac, av);
	if (exit_code)
		return (!!exit_code);
	if (args.number_of_philos == 1)
		return (handle_one(args));
	return (!!start(args));
}
