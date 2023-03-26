/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:10:39 by mfaussur          #+#    #+#             */
/*   Updated: 2023/03/26 18:56:24 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	current_time(unsigned long start)
{
	struct timeval							time;

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
