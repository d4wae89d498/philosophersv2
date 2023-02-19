/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:51 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 19:35:01 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	watcher_tick_tick(t_watcher_args *watcher_args, int i, int *y,
		int *sexit)
{
	unsigned long	last_eat;

	pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
	last_eat = (unsigned long)(watcher_args->philos_ctx[i].last_eat_time);
	pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));


	if (watcher_args->philos_ctx[i].state == plus_faim)
		*y += 1;
	else if (current_time(watcher_args->philos_ctx[i].start)
		- last_eat
			> (unsigned long)(watcher_args->args.time_to_die) * 1000
			&& watcher_args->philos_ctx[i].state != manger
			&& ++*sexit)
	{
		pthread_mutex_lock(watcher_args->philos_ctx[i].console);
		philo_msg(watcher_args->args.number_of_philos, current_time(watcher_args->args.start) * 0.001, i + 1, "died");
		return (1);
	}
	return (0);
}

int	watcher_tick(t_watcher_args *watcher_args, int *sexit)
{
	int	y;
	int	i;

	y = 0;
	i = -1;
	usleep(4444);
	while (!*sexit && (++i < watcher_args->args.number_of_philos))
		if (watcher_tick_tick(watcher_args, i, &y, sexit))
			return (1);
	if (y == watcher_args->args.number_of_philos)
		return (1);
	return (0);
}

void	*watch_philos(void *data)
{
	t_watcher_args	*watcher_args;
	int				sexit;

	watcher_args = data;
	sexit = 0;
	while (!sexit)
		if (watcher_tick(watcher_args, &sexit))
			break ;
	return (0);
}
