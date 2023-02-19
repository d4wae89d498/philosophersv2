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

void	msg(t_philo_ctx *ctx, char *msg)
{
	pthread_mutex_lock(ctx->console);
	philo_msg(ctx->args.number_of_philos, (unsigned long)(current_time(ctx->start) * 0.001),
		ctx->id, msg);
	pthread_mutex_unlock(ctx->console);
}

