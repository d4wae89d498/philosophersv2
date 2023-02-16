/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   destroy.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2023/02/16 16:50:34 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2023/02/16 16:55:22 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
/* ************************************************************************** */

#include "philo.h"

int	destroy_philos_ctx(t_philo_ctx *philos_ctx, long number_of_philos)
{
	long	i;

	i = 0;
	while (i < number_of_philos)
	{
		pthread_mutex_destroy(philos_ctx[i].right_fork);
		pthread_mutex_destroy(&(philos_ctx[i].state_mtx));
		i += 1;
	}
	return (0);
}
