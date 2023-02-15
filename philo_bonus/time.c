/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:10:39 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 13:32:23 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	current_time(void)
{
	struct timeval							time;
	static volatile unsigned long			start;

	gettimeofday(&time, 0);
	if (start == 0)
		start = ((time.tv_sec * 1000000 + time.tv_usec));
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	ft_sleep(unsigned long time)
{
	unsigned long	start;

	time *= 1000;
	start = current_time();
//	usleep(time * 0.95);
	while (current_time() < start + time)
		usleep(100);
}
