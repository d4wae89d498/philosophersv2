/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 05:58:08 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:06:28 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	unlock_both_forks(t_philo_ctx *ctx)
{
	pthread_mutex_unlock(ctx->left_fork);
	pthread_mutex_unlock(ctx->right_fork);
	return (1);
}

static int	routine_tick(t_philo_ctx *ctx)
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
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->last_eat_time = current_time(ctx->start);
	pthread_mutex_unlock(&(ctx->state_mtx));
	ctx->meals += 1;
	msg(ctx, EAT);
	ft_sleep(ctx->args.time_to_eat);
	pthread_mutex_unlock(ctx->right_fork);
	pthread_mutex_unlock(ctx->left_fork);
	msg(ctx, SLEEP);
	if (sleep_while_check_dead(ctx, ctx->args.time_to_sleep))
		return (1);
	msg(ctx, THINK);
	return (0);
}

void	*philo_routine(void *data)
{
	t_philo_ctx		*ctx;

	ctx = data;
	msg(ctx, THINK);
	if (!(ctx->id % 2))
		usleep(ctx->args.number_of_philos * START_MUL + START_DELAY);
	while (ctx->meals < ctx->args.number_of_meals
		|| (ctx->args.number_of_meals < 0))
		if (routine_tick(ctx))
			break ;
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = END;
	pthread_mutex_unlock(&(ctx->state_mtx));
	return (0);
}
