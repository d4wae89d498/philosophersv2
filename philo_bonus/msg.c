/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:54:16 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 16:00:25 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	ft_strlen(const char *s)
{
	unsigned int	i;

	i = 0;
	while (*s++)
		i += 1;
	return (i);
}

void	philo_msg(unsigned long time, unsigned int id, char *msg)
{
	char	s[32];

	ultoa(s, time);
	write(1, s, ft_strlen(s));
	write(1, "\tphilo ", 6);

	ultoa(s, id);
	write(1, s, ft_strlen(s));
	write(1, "\t", 1);

	write(1, msg, ft_strlen(msg));
	write(1, "\n", 1);
}
