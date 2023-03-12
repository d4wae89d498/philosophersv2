/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:42:51 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/24 20:32:16 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	watcher_tick_tick(t_watcher_args *watcher_args, int i, int *y)
{
	unsigned long	last_eat;
	int				current_state;

	pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
	last_eat = (unsigned long)(watcher_args->philos_ctx[i].last_eat_time);
	pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));
	pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
	current_state = watcher_args->philos_ctx[i].state;
	pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));
	if (current_state == END)
		*y += 1;
	else if (current_time(watcher_args->philos_ctx[i].start)
		- last_eat
		> (unsigned long)(watcher_args->args.time_to_die) * 1000)
	{
		watcher_args->dead = 1;
		msg(watcher_args->philos_ctx + i, DIE);
		
		pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
		current_state = watcher_args->philos_ctx[i].state;
		pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));

		if (get_state(&(watcher_args->philos_ctx[i])) == WAIT_LEFT 
			|| get_state(&(watcher_args->philos_ctx[i])) == WAIT_RIGHT)
			pthread_mutex_unlock(watcher_args->philos_ctx[i].left_fork);
		if (get_state(&(watcher_args->philos_ctx[i])) == WAIT_RIGHT)
			pthread_mutex_unlock(watcher_args->philos_ctx[i].right_fork);
	}
	return (0);
}

static int	watcher_tick(t_watcher_args *watcher_args)
{
	int	y;
	int	i;

	y = 0;
	i = -1;
	usleep(WATCHER_SLEEP);
	while (++i < watcher_args->args.number_of_philos)
		if (watcher_tick_tick(watcher_args, i, &y))
			return (1);
	if (y == watcher_args->args.number_of_philos)
	{
		philo_msg(0, 0, 0, 0);
		return (1);
	}
	return (0);
}

static void	*watch_philos(void *data)
{
	t_watcher_args	*watcher_args;

	watcher_args = data;
	while (1)
		if (watcher_tick(watcher_args))
			break ;
	return (0);
}

 int	start_watcher(t_args args, pthread_t *philos,
		t_philo_ctx *philos_ctx)
{
	static pthread_t		watcher;
	static t_watcher_args	watcher_args;

	watcher_args = (t_watcher_args){.args = args, .philos = philos,
		.philos_ctx = philos_ctx, .dead = 0};
	if (pthread_create(&watcher, 0, &watch_philos, &watcher_args))
		return (!!ft_eputs("Error: pthread_create.\n"));
	if (pthread_join(watcher, 0))
		return (!!ft_eputs("Error: pthread_join.\n"));
	return (0);
}
