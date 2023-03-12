/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:18:10 by mafaussu          #+#    #+#             */
/*   Updated: 2023/03/03 22:17:40 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_args args, unsigned long start_time, pid_t id,
		t_sems sems)
{
	(void) args;
	msg(sems.console, start_time, id, "is thinking");
	sem_wait(sems.forks);
	msg(sems.console, start_time, id, "has taken a fork");
	sem_wait(sems.forks);
	msg(sems.console, start_time, id, "has taken a fork");
	msg(sems.console, start_time, id, "is eating");
	return (0);
}

static void	drop_forks(t_args args, unsigned long start_time, pid_t id,
		t_sems sems)
{
	sem_post(sems.forks);
	sem_post(sems.forks);
	msg(sems.console, start_time, id, "is sleeping");
	ft_sleep(args.time_to_sleep);
}

static void	routine(t_args args, pid_t id, unsigned long *last_meal,
			unsigned long start_time)
{
	long			meals;
	int				posted;

	posted = 0;
	meals = 0;
	while (1)
	{
		take_forks(args, start_time, id, args.sems);
		sem_wait(args.sems.last_meal);
		*last_meal = current_time(start_time);
		sem_post(args.sems.last_meal);
		ft_sleep(args.time_to_eat);
		meals += 1;
		if (meals >= args.number_of_meals && args.number_of_meals > 0)
		{
			if (!posted)
			{
				sem_post(args.sems.remaining_eat);
				posted = 1;
			}
		}
		drop_forks(args, start_time, id, args.sems);
	}
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
	pthread_create(&watcher, 0, &watch_famine, &watcher_args);
	routine(args, i + 1, &last_meal, start_time);
	ft_sleep(args.time_to_eat + args.time_to_sleep + args.number_of_philos);
	sem_post(sems.dead);
	pthread_join(watcher, 0);
	sem_close(sems.last_meal);
	sem_unlink(last_meal_sem_label);
}
