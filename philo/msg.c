/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:37 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 15:24:57 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

char	*get_msg(t_state state)
{
	if (state == TAKE)
		return ("has taken a fork");
	if (state == EAT)
		return ("is eating");
	else if (state == SLEEP)
		return ("is sleeping");
	else if (state == THINK)
		return ("is thinking");
	else if (state == DIE)
		return ("died");
	return ("");
}

int	msg(t_philo_ctx *ctx, t_state state)
{
	int				r;
	static int		fulls;

	r = 0;
	pthread_mutex_lock(ctx->dead_mtx);
	if (state == EAT)
	{
		pthread_mutex_lock(ctx->state_mtx);
		if (ctx->meals == ctx->args.number_of_meals)
		{
			fulls += 1;
			ctx->state = END;
		}
		pthread_mutex_unlock(ctx->state_mtx);
	}
	r = *(ctx->dead);
	if ((!r && state == DIE))
		*(ctx->dead) = 1;
	if (r)
	{
		pthread_mutex_unlock(ctx->dead_mtx);
		return (1);
	}
	pthread_mutex_lock(ctx->console_mtx);
	philo_msg(ctx->args.number_of_philos,
			(unsigned long)(current_time(ctx->start) * 0.001),
			ctx->id, get_msg(state));
	pthread_mutex_unlock(ctx->console_mtx);
	if (fulls >= ctx->args.number_of_philos && state == EAT)
	{
		*(ctx->dead) = 1;
		r = 1;
	}
	pthread_mutex_unlock(ctx->dead_mtx);
	return (r);
}

void	philo_msg(long number_of_philos, unsigned long time, unsigned int id,
		char *msg)
{
	static char				buffer[MAX_THREADS * MC + 800];
	static int				i;

	if (!msg)
	{
		if (write(1, buffer, i) < 0)
		{
		}
		i = 0;
		return ;
	}
	i += ultoa(buffer + i, time);
	i += ft_sputs(buffer + i, " ");
	i += ultoa(buffer + i, id);
	i += ft_sputs(buffer + i, " ");
	i += ft_sputs(buffer + i, msg);
	i += ft_sputs(buffer + i, "\n");
	if (i > number_of_philos * MC)
	{
		if (write(1, buffer, i) < 0)
		{
		}
		i = 0;
		return ;
	}
}
