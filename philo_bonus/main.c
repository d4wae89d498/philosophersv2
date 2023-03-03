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

static void	*wait_childs(void *d)
{
	sem_t	*dead;

	dead = d;
	while (waitpid(-1, 0, 0) > -1)
		;
	sem_post(dead);
	return (0);
}


static int	wait_til_end(t_args args, t_sems sems, pid_t childs[MAX_PROCESS])
{
	char		s[255];
	pthread_t	t;
	int			r;
	long		i;

	pthread_create(&t, 0, &wait_childs, sems.dead);
	sem_wait(sems.dead);
	i = 0;
	while (i < args.number_of_philos)
	{
		kill(childs[i], SIGKILL);
		s[0] = 'd';
		s[1] = 'd';
		ultoa(s + 2, i);
		sem_unlink(s);
		i += 1;
	}
	r = 0;
	if (pthread_join(t, 0))
		r += ft_eputs("Error: pthread_join.\n");
	if (destroy_sems(&sems))
		r += ft_eputs("Error: destroy_sems\n");
	return (r);
}

static int	start(t_args args)
{
	t_sems			sems;
	pid_t			pid;
	long			i;
	unsigned long	start_time;
	pid_t			childs[MAX_PROCESS];	

	if (args.number_of_philos == 1)
		return (handle_one(args));
	if (init_sems(&sems, args.number_of_philos))
		return (0);
	start_time = current_time(0);
	i = -1;
	while (++i < args.number_of_philos)
	{
		pid = fork();
		if (!pid)
		{
			start_routine(args, sems, start_time, i);
			exit(0);
		}
		childs[i] = pid;
	}
	return (wait_til_end(args, sems, childs));
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
	t_args args;
	int exit_code;

	exit_code = parse(&args, ac, av);
	if (exit_code)
		return (!!exit_code);
	return (!!start(args));
}
