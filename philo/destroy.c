/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:05 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 12:08:56 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	destroy_philos_ctx(t_philo_ctx *philos_ctx, long number_of_philos)
{
	long	i;

	i = 0;
	while (i < number_of_philos)
	{
		mtx_destroy(philos_ctx[i].right_fork);
		pthread_mutex_destroy(&(philos_ctx[i].state_mtx));
		i += 1;
	}
	return (0);
}
