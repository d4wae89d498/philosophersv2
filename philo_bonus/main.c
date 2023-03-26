/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:15:20 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		|| args->number_of_philos > MAX_PROCESS)
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
