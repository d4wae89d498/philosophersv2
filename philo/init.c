/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:16:48 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/24 20:30:25 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
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
			return (!!ft_eputs("Error: mtx_init.\n"));
		}
		i += 1;
	}
	return (0);
}

static void	set_fork(t_philo_ctx *ctx, pthread_mutex_t *table, t_args args,
		long i)
{
	if (i + 1 == args.number_of_philos)
	{
		ctx->left_fork = table + i;
		ctx->right_fork = table + 0;
	}
	else
	{
		ctx->left_fork = table + i;
		ctx->right_fork = table + i + 1;
	}
}

int	init_philos_ctx(t_args args, pthread_mutex_t *table, t_philo_ctx *philos_ctx,
		pthread_mutex_t *console)
{
	long					i;
	static int				dead;

	dead = 0;
	i = -1;
	while (++i < args.number_of_philos)
	{
		philos_ctx[i] = (t_philo_ctx){
			.dead = &dead,
			.id = i + 1,
			.state = THINK,
			.start = args.start,
			.last_eat_time = 0,
			.console = console,
			.args = args,
			.meals = 0
		};
		if (pthread_mutex_init(&(philos_ctx[i].state_mtx), 0))
			return (!!ft_eputs("Error: pthread_mutex_init.\n"));
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
			return (!!ft_eputs("Error: pthread_create.\n"));
		i += 1;
	}
	return (0);
}
