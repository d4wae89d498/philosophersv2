/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 05:58:08 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 06:04:09 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	try_eat(t_philo_ctx *ctx)
{
	pthread_mutex_lock(ctx->left_fork);
	pthread_mutex_lock(&(ctx->state_mtx));
	if (ctx->dead)
		return (!!(pthread_mutex_unlock(&(ctx->state_mtx)) + 1));
	pthread_mutex_unlock(&(ctx->state_mtx));
	msg(ctx, "has taken a fork");
	pthread_mutex_lock(ctx->right_fork);
	pthread_mutex_lock(&(ctx->state_mtx));
	if (ctx->dead)
		return (!!(pthread_mutex_unlock(&(ctx->state_mtx)) + 1));
	ctx->state = manger;
	ctx->last_eat_time = current_time(ctx->start);
	ctx->meals += 1;
	pthread_mutex_unlock(&(ctx->state_mtx));
	msg(ctx, "has taken a fork");
	msg(ctx, "is eating");
	ft_sleep(ctx->args.time_to_eat);
	pthread_mutex_unlock(ctx->right_fork);
	pthread_mutex_unlock(ctx->left_fork);
	return (0);
}

static int	a_plus_faim(t_philo_ctx *ctx)
{
	return (ctx->meals >= ctx->args.number_of_meals
		&& (ctx->args.number_of_meals > 0));
}

static int	routine_sleep(t_philo_ctx *ctx)
{
	int	cond;

	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = dormir;
	pthread_mutex_unlock(&(ctx->state_mtx));
	msg(ctx, "is sleeping");
	ft_sleep(ctx->args.time_to_sleep);
	pthread_mutex_lock(&(ctx->state_mtx));
	cond = ctx->dead;
	pthread_mutex_unlock(&(ctx->state_mtx));
	if (cond)
		return (1);
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = penser;
	pthread_mutex_unlock(&(ctx->state_mtx));
	msg(ctx, "is thinking");
	return (0);
}

static int	routine_tick(t_philo_ctx *ctx)
{
	int				cond;

	pthread_mutex_lock(&(ctx->state_mtx));
	cond = ctx->dead;
	pthread_mutex_unlock(&(ctx->state_mtx));
	if (cond)
		return (1);
	if (try_eat(ctx))
		return (1);
	pthread_mutex_lock(&(ctx->state_mtx));
	cond = a_plus_faim(ctx);
	pthread_mutex_unlock(&(ctx->state_mtx));
	if (cond)
		return (1);
	return (routine_sleep(ctx));
}

void	*philo_routine(void *data)
{
	t_philo_ctx		*ctx;

	ctx = data;
	if (ctx->id % 2)
		ft_sleep(1);
	while (ctx->meals < ctx->args.number_of_meals
		|| (ctx->args.number_of_meals < 0))
		if (routine_tick(ctx))
			break ;
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = plus_faim;
	pthread_mutex_unlock(&(ctx->state_mtx));
	return (0);
}
