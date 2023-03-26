/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:15:20 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*wait_childs(void *data)
{
	t_end_args	*watcher_args;

	watcher_args = data;
	while (waitpid(-1, 0, 0) > -1)
		sem_post(watcher_args->remaining);
	sem_post(watcher_args->dead);
	return (0);
}

static void	*wait_end(void *data)
{
	t_end_args	*watcher_args;
	long		i;

	watcher_args = data;
	i = 0;
	while (i < watcher_args->number_of_philos)
	{
		sem_wait(watcher_args->remaining);
		i += 1;
	}
	sem_post(watcher_args->dead);
	return (0);
}

static int	join_threads(pthread_t t, pthread_t t2, t_sems *sems)
{
	int		r;
	void	*exit_code;

	r = 0;
	if (pthread_join(t, &exit_code))
		r += ft_eputs("Error: pthread_join.\n");
	if (pthread_join(t2, &exit_code))
		r += ft_eputs("Error: pthread_join.\n");
	if (destroy_sems(sems))
		r += ft_eputs("Error: destroy_sems\n");
	return (r);
}

static int	wait_til_end(t_args args, t_sems sems, pid_t childs[MAX_PROCESS])
{
	char		s[255];
	pthread_t	t;
	pthread_t	t2;
	long		i;
	t_end_args	end_args;

	end_args = (t_end_args){.dead = sems.dead, .remaining = sems.remaining_eat,
		.number_of_philos = args.number_of_philos, .console = sems.console};
	pthread_create(&t2, 0, &wait_childs, &end_args);
	pthread_create(&t, 0, &wait_end, &end_args);
	sem_wait(sems.dead);
	sem_wait(sems.console);
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
	return (join_threads(t, t2, &sems));
}

int	start(t_args args)
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
