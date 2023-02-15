/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:41:32 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 16:03:51 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ultoa(char *o, unsigned long n)
{
	unsigned char	pow;
	unsigned long	bkp;

	pow = 0;
	bkp = n;
	o[0] = '0';
	o[1] = 0;
	while (bkp)
	{
		bkp /= 10;
		pow += 1;
	}
	if (pow)
		o[pow] = 0;
	while (pow)
	{
		o[--pow] = n % 10 + '0';
		n /= 10;
	}
}

static unsigned int	ft_strlen(const char *s)
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
	write(1, " philo ", 7);

	ultoa(s, id);
	write(1, s, ft_strlen(s));
	write(1, " ", 1);

	write(1, msg, ft_strlen(msg));
	write(1, "\n", 1);
}
