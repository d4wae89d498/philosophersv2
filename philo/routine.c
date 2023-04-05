/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 05:58:08 by mfaussur          #+#    #+#             */
/*   Updated: 2023/04/05 10:26:43 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline int	unlock_both_forks(t_philo_ctx *ctx)
{
	pthread_mutex_unlock(ctx->left_fork);
	pthread_mutex_unlock(ctx->right_fork);
	return (1);
}

static inline int	routine_tick_eat(t_philo_ctx *ctx)
{
	pthread_mutex_lock(ctx->left_fork);
	if (msg(ctx, TAKE))
	{
		pthread_mutex_unlock(ctx->left_fork);
		return (1);
	}
	pthread_mutex_lock(ctx->right_fork);
	if (msg(ctx, TAKE))
	{
		unlock_both_forks(ctx);
		return (1);
	}
	pthread_mutex_lock(ctx->state_mtx);
	ctx->meals += 1;
	ctx->last_eat_time = current_time(ctx->start);
	pthread_mutex_unlock(ctx->state_mtx);
	if (msg(ctx, EAT))
	{
		unlock_both_forks(ctx);
		return (1);
	}
	ft_sleep(ctx->args.time_to_eat);
	pthread_mutex_unlock(ctx->right_fork);
	pthread_mutex_unlock(ctx->left_fork);
	return (0);
}

static inline int	routine_tick(t_philo_ctx *ctx)
{
	routine_tick_eat(ctx);
	if (msg(ctx, SLEEP))
		return (1);
	if (sleep_while_check_dead(ctx, ctx->args.time_to_sleep))
		return (1);
	if (msg(ctx, THINK))
		return (1);
	ft_usleep(THINK_MIN_DELAY);
	return (0);
}

void	*philo_routine(void *data)
{
	t_philo_ctx		*ctx;

	ctx = data;
	//ft_sleep(0.2 * (ctx->args.number_of_philos - ctx->id));
	msg(ctx, THINK);
	if (ctx->id % 2 & NUMBER_OF_PHILOS_START_FACTOR & START_DELAY)
		ft_usleep(ctx->args.number_of_philos * NUMBER_OF_PHILOS_START_FACTOR 
			+ START_DELAY);
	while (ctx->meals < ctx->args.number_of_meals
		|| (ctx->args.number_of_meals < 0))
		if (routine_tick(ctx))
			break ;
	pthread_mutex_lock(ctx->state_mtx);
	ctx->state = END;
	pthread_mutex_unlock(ctx->state_mtx);
	return (0);
}
