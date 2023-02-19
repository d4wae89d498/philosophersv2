/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 05:58:08 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/19 19:34:13 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	routine_tick(t_philo_ctx *ctx)
{
	pthread_mutex_lock(ctx->left_fork);
	msg(ctx, "has taken a fork");
	pthread_mutex_lock(ctx->right_fork);
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->last_eat_time = current_time(ctx->start);
	pthread_mutex_unlock(&(ctx->state_mtx));
	ctx->meals += 1;
	msg(ctx, "has taken a fork");
	msg(ctx, "is eating");
	ft_sleep(ctx->args.time_to_eat);
	pthread_mutex_unlock(ctx->right_fork);
	pthread_mutex_unlock(ctx->left_fork);
	if (ctx->meals >= ctx->args.number_of_meals
		&& (ctx->args.number_of_meals > 0))
		return (1);
	msg(ctx, "is sleeping");
	ft_sleep(ctx->args.time_to_sleep);
	msg(ctx, "is thinking");
	return (0);
}

void	*philo_routine(void *data)
{
	t_philo_ctx		*ctx;

	ctx = data;
	if (ctx->id % 2)
		usleep(10000);
	while (ctx->meals < ctx->args.number_of_meals
		|| (ctx->args.number_of_meals < 0))
		if (routine_tick(ctx))
			break ;
	pthread_mutex_lock(&(ctx->state_mtx));
	ctx->state = plus_faim;
	pthread_mutex_unlock(&(ctx->state_mtx));
	return (0);
}
