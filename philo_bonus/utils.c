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

long	ft_atol(char *s)
{
	long	n;

	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		n *= 10;
		n += *s++ - '0';
	}
	if (*s)
		return (-1);
	return (n);
}

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

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s++)
		i += 1;
	return (i);
}

int	ft_eputs(const char *s)
{
	int	i;

	i = ft_strlen(s);
	write(2, s, i);
	return (i);
}

int	ft_puts(const char *s)
{
	int	i;

	i = ft_strlen(s);
	write(1, s, i);
	return (i);
}
