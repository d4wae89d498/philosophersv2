/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   watcher.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2023/02/16 08:48:51 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2023/02/16 18:16:21 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
/* ************************************************************************** */

#include "philo.h"

static void	watcher_tick_tick(t_watcher_args *watcher_args, int i, int *y,
		int *sexit)
{
	pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
	if (watcher_args->philos_ctx[i].state == plus_faim)
		*y += 1;
	else if (current_time(watcher_args->philos_ctx[i].start)
		- (unsigned long)(watcher_args->philos_ctx[i].last_eat_time)
			> (unsigned long)(watcher_args->args.time_to_die) * 1000
			&& watcher_args->philos_ctx[i].state != manger
			&& ++*sexit)
	{
		watcher_args->philos_ctx[i].dead = 1;
		pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));
		pthread_mutex_lock(watcher_args->philos_ctx[i].gdead_mtx);
		*(watcher_args->philos_ctx[i].gdead) = 1;
		pthread_mutex_unlock(watcher_args->philos_ctx[i].gdead_mtx);
		ft_sleep(1);
		dead_msg(watcher_args->philos_ctx + i, "died");
		pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
	}
	pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));
}

int	watcher_tick(t_watcher_args *watcher_args, int *sexit)
{
	int	y;
	int	i;

	y = 0;
	i = -1;
	while (!*sexit && (++i < watcher_args->args.number_of_philos))
		watcher_tick_tick(watcher_args, i, &y, sexit);
	if (y == watcher_args->args.number_of_philos)
		return (1);
	ft_sleep(1);
	return (0);
}

void	*watch_philos(void *data)
{
	t_watcher_args	*watcher_args;
	long			i;
	int				sexit;

	watcher_args = data;
	sexit = 0;
	while (!sexit)
		if (watcher_tick(watcher_args, &sexit))
			break ;
	i = 0;
	while (i < watcher_args->args.number_of_philos)
	{	
		pthread_mutex_unlock(watcher_args->philos_ctx[i].left_fork);
		pthread_mutex_unlock(watcher_args->philos_ctx[i].right_fork);
		pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
		watcher_args->philos_ctx[i].dead = 1;
		pthread_mutex_unlock(&(watcher_args->philos_ctx[i].state_mtx));
		i += 1;
	}
	return (0);
}
