/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 23:01:14 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*wait_childs(void *d)
{
	sem_t	*dead;

	dead = d;
	while (waitpid(-1, 0, 0) > -1)
		;
	sem_post(d);
	return (0);
}

void	start_routine(t_args args, t_sems sems, unsigned long start_time,
			long i)
{
	pthread_t						watcher;
	static unsigned long			last_meal;	
	t_watcher_args					watcher_args;
	char							last_meal_sem_label[255];

	last_meal_sem_label[0] = 'd';
	last_meal_sem_label[1] = 'd';
	ultoa(last_meal_sem_label + 2, i);
	sem_unlink(last_meal_sem_label);
	sems.last_meal = sem_open(last_meal_sem_label, O_CREAT, 0644, 1);
	if (sems.last_meal == SEM_FAILED)
		exit(sem_post(sems.dead));
	args.sems = sems;
	last_meal = start_time;
	watcher_args = (t_watcher_args){
		.last_meal_sem = sems.last_meal, .start_time = start_time,
		.dead = sems.dead, .last_meal = &last_meal, .id = i + 1, .args = args,
		.console = sems.console
	};
	pthread_create(&watcher, 0, &watch, &watcher_args);
	routine(args, i + 1, &last_meal, start_time);
	sem_post(sems.dead);
	pthread_join(watcher, 0);
	sem_close(sems.last_meal);
	sem_unlink(last_meal_sem_label);
}

int	wait_til_end(t_args args, t_sems sems, pid_t childs[MAX_PROCESS])
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
		r += !!ft_puts("Error: pthread_join.\n");
	if (destroy_sems(&sems))
		r += !!ft_puts("Error: destroy_sems\n");
	return (!!r);
}

int	start(t_args args)
{
	t_sems			sems;
	pid_t			pid;
	long			i;
	unsigned long	start_time;
	pid_t			childs[MAX_PROCESS];	

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

int	main(int ac, char **av)
{
	static t_args	args;

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
