/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:41:32 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 06:46:31 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_puts(char *str)
{
	int	len;

	len = ft_strlen(str);
	write(1, str, len);
	return (len);
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

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s++)
		i += 1;
	return (i);
}

void	philo_msg(unsigned long time, unsigned int id, char *msg)
{
	char	s[32];

	ultoa(s, time);
	ft_puts(s);
	ft_puts(" philo ");
	ultoa(s, id);
	ft_puts(s);
	ft_puts(" ");
	write(1, msg, ft_strlen(msg));
	ft_puts("\n");
}
