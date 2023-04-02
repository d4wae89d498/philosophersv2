/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/02 15:15:33 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 17:15:17 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_simulation(t_dinning_simulation *sim)
{
	if (pthread_mutex_init(&(sim->console_mtx), 0))
	{
		ft_eputs("Error: pthread_mutex_init.\n");
		return (1);
	}
	if (pthread_mutex_init(&(sim->dead_mtx), 0))
	{
		ft_eputs("Error: pthread_mutex_init.\n");
		return (destroy_mutex(&(sim->console_mtx)));
	}
	sim->args.start = current_time(0);
	if (init_philo_ctx(sim))
	{
		ft_eputs("Error: init_philo_ctx.\n");
		return ((destroy_mutex(&(sim->console_mtx))
				+ destroy_mutex(&(sim->dead_mtx)) + 1));
	}
	assign_forks(sim);
	return (0);
}

static int	destroy_simulation(t_dinning_simulation *sim)
{
	return ((destroy_mutex(&(sim->console_mtx)))
		+ destroy_mutex(&(sim->dead_mtx))
		+ destroy_philo_ctx(sim));
}

static int	launch_simulation(t_dinning_simulation *sim)
{
	void	*exit_status;
	long	i;

	if (start_philos(sim))
	{
		ft_eputs("Error: start_philos.\n");
		return (destroy_simulation(sim) + 1);
	}
	if (start_watcher(sim))
	{
		ft_eputs("Error: start_watcher.\n");
		pthread_mutex_lock(&(sim->dead_mtx));
		*(sim->philo_ctx[0].dead) = 1;
		pthread_mutex_unlock(&(sim->dead_mtx));
		i = 0;
		while (i < sim->args.number_of_philos)
			pthread_join(sim->philo_thread[i++], &exit_status);
		return (destroy_simulation(sim) + 1);
	}
	while ((sim->args.number_of_philos)--)
		pthread_join(sim->philo_thread[sim->args.number_of_philos],
			&exit_status);
	philo_msg(0, 0, 0, 0);
	return (0);
}

int	start_simulation(t_args args)
{
	t_dinning_simulation	sim;

	sim.args = args;
	if (init_simulation(&sim))
		return (1);
	if (launch_simulation(&sim))
		return (1);
	if (destroy_simulation(&sim))
		return (1);
	return (0);
}
