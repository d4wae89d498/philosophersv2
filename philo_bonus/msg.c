/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:18:01 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 16:10:35 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline char	*get_msg(t_msg state)
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

void	philo_msg(unsigned long time, unsigned int id, char *msg)
{
	char	s[32];

	ultoa(s, time);
	ft_puts(s);
	ft_puts(" ");
	ultoa(s, id);
	ft_puts(s);
	ft_puts(" ");
	ft_puts(msg);
	ft_puts("\n");
}

void	msg(sem_t *console, unsigned long start_time, int id, t_msg state)
{
	if (console)
		sem_wait(console);
	philo_msg((unsigned long)(current_time(start_time) / 1000), id,
		get_msg(state));
	if (console)
		sem_post(console);
}
