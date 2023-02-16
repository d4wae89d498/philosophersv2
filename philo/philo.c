/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   philo.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2023/02/16 08:48:51 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2023/02/16 09:12:21 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
/* ************************************************************************** */

#include "philo.h"

int	init_table(pthread_mutex_t *table, long number_of_philos)
{
	long	i;

	i = 0;
	while (i < number_of_philos)
	{
		if (pthread_mutex_init(table + i, 0))
		{
			while (i--)
				pthread_mutex_destroy(table + i);
			return (1);
		}
		i += 1;
	}
	return (0);
}

void	init_philos_ctx(t_args args, pthread_mutex_t *table,
		t_philo_ctx *philos_ctx, t_mutexes *m)
{
	long					i;
	static volatile int		gdead;
	static unsigned long	start;

	start = current_time(0);
	gdead = 0;
	i = 0;
	while (i < args.number_of_philos)
	{
		pthread_mutex_init(&(philos_ctx[i].state_mtx), 0);
		philos_ctx[i] = (t_philo_ctx){.id = i + 1, .state = penser,
			.gdead_mtx = &(m->gdead_mtx),
			.start = start,
			.last_eat_time = 0,
			.console = &(m->console),
			.dead_console = &(m->dead_console),
			.args = args,
			.meals = 0,
			.dead = 0,
			.gdead = &gdead
		};
		if (i == args.number_of_philos - 1)
		{
			philos_ctx[i].left_fork = table + args.number_of_philos - 1;
			philos_ctx[i].right_fork = table + 0;
		}
		else
		{
			philos_ctx[i].left_fork = table + i;
			philos_ctx[i].right_fork = table + i + 1;
		}
		i += 1;
	}
	return ;
}

int	init_philos(long number_of_philos, t_philo_ctx *philos_ctx,
		pthread_t *philos)
{
	long		i;

	i = 0;
	while (i < number_of_philos)
	{
		if (pthread_create(philos + i, 0, &philo_routine, philos_ctx + i))
		{
			while (i--)
				philos_ctx[i].dead = 1;
			return (1);
		}
		i += 1;
	}
	return (0);
}

int	watcher_tick(t_watcher_args *watcher_args, int *sexit)
{
	int	y;
	int	i;

	y = 0;
	i = -1;
	while (!*sexit && (++i < watcher_args->args.number_of_philos))
	{
		pthread_mutex_lock(&(watcher_args->philos_ctx[i].state_mtx));
		if (watcher_args->philos_ctx[i].state == plus_faim)
			y += 1;
		else if (current_time(watcher_args->philos_ctx[i].start)
			- (unsigned long)(watcher_args->philos_ctx[i].last_eat_time)
				> (unsigned long)(watcher_args->args.time_to_die) * 1000
				&& watcher_args->philos_ctx[i].state != manger
				&& ++sexit)
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
		pthread_mutex_unlock(&(watcher_args->philos_ctx[i++].state_mtx));
	}
	return (0);
}
