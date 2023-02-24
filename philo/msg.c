/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:37 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 17:39:55 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <string.h>

// todo : implement enum for msgs

int	msg(t_philo_ctx *ctx, char *msg)
{
	int	r;

	pthread_mutex_lock(ctx->console);
	r = *(ctx->dead);
	if (!r && msg && !strcmp(msg, "died"))
	{
		*(ctx->dead) = 1;
	}
	if (r)
	{
		pthread_mutex_unlock(ctx->console);
		return (1);
	}
	philo_msg(ctx->args.number_of_philos, (unsigned long)(current_time(ctx->start) * 0.001),
			ctx->id, msg);
	pthread_mutex_unlock(ctx->console);
	return (0);
}
