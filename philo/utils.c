/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:34:08 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:34:09 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ultoa(char *o, unsigned long n)
{
	unsigned char	pow;
	unsigned long	bkp;
	int				i;

	i = 0;
	pow = 0;
	bkp = n;
	o[0] = '0';
	o[1] = 0;
	while (bkp)
	{
		i += 1;
		bkp /= 10;
		pow += 1;
	}
	if (pow)
		o[pow] = 0;
	else
		i = 1;
	while (pow)
	{
		o[--pow] = n % 10 + '0';
		n /= 10;
	}
	return (i);
}

long	ft_atol(char *s)
{
	long	n;

	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		n *= 10;
		n += *s++ - '0';
		if (n > INT_MAX || n < INT_MIN)
			return (-1);
	}
	if (*s)
		return (-1);
	return (n);
}
