/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:37 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/24 20:31:23 by mfaussur         ###   ########lyon.fr   */
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
	philo_msg(ctx->args.number_of_philos,
		(unsigned long)(current_time(ctx->start) * 0.001),
		ctx->id, get_msg(state));
	pthread_mutex_unlock(ctx->console);
	return (0);
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
