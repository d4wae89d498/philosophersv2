/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:37 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/24 19:41:49 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>

char	*get_msg(t_state state)
{
	if (state == TAKE)
		return ("has taked a fork");
	if (state == EAT)
		return ("is eating");
	else if (state == SLEEP)
		return ("is sleeping");
	else if (state == THINK)
		return ("is thinking");
	else if (state == DIE)
		return ("died");
	return ("");
};

int	msg(t_philo_ctx *ctx, t_state state)
{
	int	r;

	pthread_mutex_lock(ctx->console);
	r = *(ctx->dead);
	if (!r && state == DIE)
	{
		*(ctx->dead) = 1;
	}
	if (r)
	{
		pthread_mutex_unlock(ctx->console);
		return (1);
	}
	philo_msg(ctx->args.number_of_philos, (unsigned long)(current_time(ctx->start) * 0.001),
			ctx->id, get_msg(state));
	pthread_mutex_unlock(ctx->console);
	return (0);
}
