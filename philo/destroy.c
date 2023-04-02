/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:05 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 15:09:52 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_mutex(pthread_mutex_t *mtx)
{
	pthread_mutex_destroy(mtx);
	return (1);
}

int	destroy_philo_ctx(t_dinning_simulation *sim)
{
	long	i;

	i = 0;
	while (i < sim->args.number_of_philos)
	{
		pthread_mutex_destroy(sim->fork_mtx + i);
		pthread_mutex_destroy(sim->state_mtx + i);
		i += 1;
	}
	return (0);
}
