/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:18:10 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 11:18:11 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	take_forks(t_args args, pid_t id, unsigned long start_time,
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

static void	drop_forks(t_args args, pid_t id, unsigned long start_time,
		t_sems sems)
{
	sem_post(sems.forks);
	sem_post(sems.forks);
	msg(sems.console, start_time, id, "is sleeping");
	ft_sleep(args.time_to_sleep);
}

void	routine(t_args args, pid_t id, unsigned long *last_meal,
			unsigned long start_time)
{
	long			meals;

	if (id % 2)
		ft_sleep(1);
	meals = 0;
	while (1)
	{
		take_forks(args, id, start_time, args.sems);
		msg(args.sems.console, start_time, id, "is eating");
		sem_wait(args.sems.last_meal);
		*last_meal = current_time(start_time);
		sem_post(args.sems.last_meal);
		ft_sleep(args.time_to_eat);
		meals += 1;
		if (meals >= args.number_of_meals && args.number_of_meals > 0)
		{
			sem_wait(args.sems.last_meal);
			*last_meal = LONG_MAX;
			sem_post(args.sems.last_meal);
			sem_post(args.sems.forks);
			sem_post(args.sems.forks);
			return ;
		}
		drop_forks(args, id, start_time, args.sems);
	}
}
