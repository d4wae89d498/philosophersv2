/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:18:01 by mafaussu          #+#    #+#             */
/*   Updated: 2023/02/19 11:18:02 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_msg(unsigned long time, unsigned int id, char *msg)
{
	char	s[32];

	ultoa(s, time);
	ft_puts(s);
	ft_puts(" philo ");
	ultoa(s, id);
	ft_puts(s);
	ft_puts(" ");
	ft_puts(msg);
	ft_puts("\n");
}

void	msg(sem_t *console, unsigned long start_time, int id, char *msg)
{
	if (console)
		sem_wait(console);
	philo_msg((unsigned long)(current_time(start_time) / 1000), id, msg);
	if (console)
		sem_post(console);
}
