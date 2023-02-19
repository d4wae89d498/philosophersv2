/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:37 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 12:51:39 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msg(t_philo_ctx *ctx, char *msg)
{
	pthread_mutex_lock(ctx->gdead_mtx);
	if (*(ctx->gdead))
	{
		pthread_mutex_unlock(ctx->gdead_mtx);
		return ;
	}
	pthread_mutex_unlock(ctx->gdead_mtx);
	pthread_mutex_lock(ctx->console);
	philo_msg((unsigned long)(current_time(ctx->start) * 0.001),
		ctx->id, msg);
	pthread_mutex_unlock(ctx->console);
}

void	dead_msg(t_philo_ctx *ctx, char *msg)
{
	pthread_mutex_lock(ctx->dead_console);
	philo_msg((unsigned long)(current_time(ctx->start) * 0.001), ctx->id, msg);
	pthread_mutex_unlock(ctx->dead_console);
}
