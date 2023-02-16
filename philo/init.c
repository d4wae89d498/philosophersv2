/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2023/02/16 10:00:51 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2023/02/16 18:15:27 by marvin      |____\_, \___/_||_| |_|      */
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
			return (!!ft_puts("Error: pthread_mutex_init.\n"));
		}
		i += 1;
	}
	return (0);
}

static void	set_fork(t_philo_ctx *ctx, pthread_mutex_t *table, t_args args,
		long i)
{
	if (i == args.number_of_philos - 1)
	{
		ctx->left_fork = table + args.number_of_philos - 1;
		ctx->right_fork = table + 0;
	}
	else
	{
		ctx->left_fork = table + i;
		ctx->right_fork = table + i + 1;
	}
}

int	init_philos_ctx(t_args args, pthread_mutex_t *table,
		t_philo_ctx *philos_ctx, t_mutexes *m)
{
	long					i;
	static volatile int		gdead;

	gdead = 0;
	i = -1;
	while (++i < args.number_of_philos)
	{
		philos_ctx[i] = (t_philo_ctx){.id = i + 1, .state = penser,
			.gdead_mtx = &(m->gdead_mtx), .start = args.start,
			.last_eat_time = 0, .console = &(m->console),
			.dead_console = &(m->dead_console), .args = args,
			.meals = 0, .dead = 0, .gdead = &gdead
		};
		if (pthread_mutex_init(&(philos_ctx[i].state_mtx), 0))
			return (!!ft_puts("Error: pthread_mutex_init.\n"));
		set_fork(philos_ctx + i, table, args, i);
	}
	return (0);
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
			return (!!ft_puts("Error: pthread_create.\n"));
		}
		i += 1;
	}
	return (0);
}
