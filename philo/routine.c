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

t_state	get_state(t_philo_ctx *ctx)
{
	t_state s;
		
	pthread_mutex_lock(&(ctx->state_mtx));
	s = ctx->state;
	pthread_mutex_unlock(&(ctx->state_mtx));
	return (s);
}

void	toggle_state(t_philo_ctx *ctx, t_state state)
{
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = state;
	pthread_mutex_unlock(&(ctx->state_mtx));
}

static int	routine_tick(t_philo_ctx *ctx)
{
	toggle_state(ctx, WAIT_LEFT);
	pthread_mutex_lock(ctx->left_fork);
	toggle_state(ctx, WAIT_RIGHT);
	if (msg(ctx, TAKE))
		return (1);
	pthread_mutex_lock(ctx->right_fork);
	if (msg(ctx, TAKE))
		return (1);
	toggle_state(ctx, NO_WAIT);
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
	if (ctx->id % 2)
		usleep(10000);
	while (ctx->meals < ctx->args.number_of_meals
		|| (ctx->args.number_of_meals < 0))
		if (routine_tick(ctx))
			break ;
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = END;
	pthread_mutex_unlock(&(ctx->state_mtx));
	return (0);
}
