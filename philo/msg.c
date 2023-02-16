/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   msg.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   Authors: marvin                                +:+   +:    +:    +:+     */
/*   <marvin@42lyon.fr>                            #+#   #+    #+    #+#      */
/*                                                #+#   ##    ##    #+#       */
/*                                               ###    #+./ #+    ###.fr     */
/*                                                        /   UNIV -          */
/*                                               | |  _  / ___ _ _   / |      */
/*   Created: 2023/02/16 09:00:53 by marvin      | |_| || / _ \ ' \  | |      */
/*   Updated: 2023/02/16 09:13:36 by marvin      |____\_, \___/_||_| |_|      */
/*                                                    /__/            .fr     */
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
