/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 06:04:30 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:06:10 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	current_time(unsigned long start)
{
	struct timeval	time;

	gettimeofday(&time, 0);
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	ft_usleep(unsigned long time)
{
	unsigned long	start;

	start = current_time(0);
	usleep(time * 97 / 100);
	while (current_time(0) < start + time)
		usleep(161);
}

void	ft_sleep(unsigned long time)
{
	if (time * 1000 < MIN_SLEEP_DELAY)
		ft_usleep(MIN_SLEEP_DELAY);
	else
		ft_usleep(time * 1000);
}

int	sleep_while_check_dead(t_philo_ctx *ctx, unsigned long time)
{
	int	r;

	while (time >= SLEEP_CHECK_DELAY)
	{
		time -= SLEEP_CHECK_TICK;
		ft_sleep(SLEEP_CHECK_TICK);
		pthread_mutex_lock(ctx->console);
		r = *(ctx->dead);
		pthread_mutex_unlock(ctx->console);
		if (r)
			return (1);
	}
	ft_sleep(time);
	return (0);
}

/*
int main()
{
	unsigned long	current;
	int	it = 10;
	int i = 0;
	unsigned long 	total;

	total = 0;
while (i < it)
{
	current = current_time(0);
	ft_usleep(800000);
	total += current_time(current) - 800000;
	i += 1;
}
	printf("%lu\n",  total / i );
}*/