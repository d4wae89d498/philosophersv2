/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 06:04:30 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/19 19:23:24 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	current_time(unsigned long start)
{
	struct timeval	time;

	gettimeofday(&time, 0);
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	ft_sleep(unsigned long time)
{
	unsigned long	start;

	time *= 1000;
	start = current_time(0);
	usleep(time * 97 / 100);
	while (current_time(0) < start + time)
		;
}

int	sleep_while_check_dead(t_philo_ctx *ctx, unsigned long time)
{
	int	r;

	while (time >= 10000)
	{
		time -= 2000;
		ft_sleep(2000);
		pthread_mutex_lock(ctx->console);
		r = *(ctx->dead);
		pthread_mutex_unlock(ctx->console);
		if (r)
			return (1);
	}
	ft_sleep(time);
	return (0);
}
