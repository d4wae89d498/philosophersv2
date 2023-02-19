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
//	while (current_time(0) + 600 < start + time)
//		usleep(500);
	while (current_time(0) < start + time)
		;
}
