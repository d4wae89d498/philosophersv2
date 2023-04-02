/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:16:48 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 17:15:01 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	assign_forks(t_dinning_simulation *sim)
{
	int	i;

	i = -1;
	while (++i < sim->args.number_of_philos)
	{
		if ((i % 2))
		{
			sim->philo_ctx[i].left_fork = sim->fork_mtx + i;
			sim->philo_ctx[i].right_fork = sim->fork_mtx + (i + 1)
				% sim->args.number_of_philos;
		}
		else
		{
			sim->philo_ctx[i].left_fork = sim->fork_mtx + (i + 1)
				% sim->args.number_of_philos;
			sim->philo_ctx[i].right_fork = sim->fork_mtx + i;
		}
	}
}

int	init_per_philo_mutexes(t_dinning_simulation *sim, long i)
{
	if (pthread_mutex_init(sim->fork_mtx + i, 0))
	{
		while (--i >= 0)
		{
			pthread_mutex_destroy(sim->fork_mtx + i);
			pthread_mutex_destroy(sim->state_mtx + i);
		}
		return (1);
	}
	if (pthread_mutex_init(sim->state_mtx + i, 0))
	{
		pthread_mutex_destroy(sim->fork_mtx + i);
		while (--i >= 0)
			pthread_mutex_destroy(sim->state_mtx + i);
		return (1);
	}
	return (0);
}

int	init_philo_ctx(t_dinning_simulation *sim)
{
	long		i;
	static int	dead;

	dead = 0;
	i = -1;
	while (++i < sim->args.number_of_philos)
	{
		sim->philo_ctx[i] = (t_philo_ctx){
			.console_mtx = &(sim->console_mtx),
			.state_mtx = sim->state_mtx + i,
			.dead_mtx = &(sim->dead_mtx),
			.id = i + 1,
			.state = THINK,
			.start = sim->args.start,
			.last_eat_time = 0,
			.args = sim->args,
			.meals = 0,
			.dead = &dead,
		};
		if (init_per_philo_mutexes(sim, i))
			return (1);
	}
	return (0);
}

int	start_philos(t_dinning_simulation *sim)
{
	long		i;
	void		*exit_status;

	i = 0;
	while (i < sim->args.number_of_philos)
	{
		if (pthread_create(sim->philo_thread + i, 0,
				&philo_routine, sim->philo_ctx + i))
		{
			pthread_mutex_lock(&(sim->dead_mtx));
			*(sim->philo_ctx[0].dead) = 1;
			pthread_mutex_unlock(&(sim->dead_mtx));
			while (--i >= 0)
				pthread_join(sim->philo_thread[i], &exit_status);
			return (1);
		}
		i += 1;
	}
	return (0);
}
