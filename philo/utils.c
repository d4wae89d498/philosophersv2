/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:41:32 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/19 19:36:48 by mafaussu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// todo : err in stderr
int	ft_dputs(int fd, char *str)
{
	int	len;

	len = ft_strlen(str);
	write(fd, str, len);
	return (len);
}

int	ft_puts(char *str)
{
	return ft_dputs(1, str);
}

int	ft_sputs(char *dst, const char *src)
{
	int	i;

	i = 0;
	while (src[i])
	{
		dst[i] = src[i];
		i += 1;
	}
	return (i);
}

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

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s++)
		i += 1;
	return (i);
}

#define MC 5 * 42
#include <string.h>

void	philo_msg(long  number_of_philos, unsigned long time, unsigned int id, char *msg)
{
	static char				buffer[MAX_THREADS * MC + 800];
	static int				i;

	if (!msg)
	{
		write(1, buffer, i);
		i = 0;
		return ;
	}
	i += ultoa(buffer + i, time);
	i += ft_sputs(buffer + i, " philo ");
	i += ultoa(buffer + i, id);
	i += ft_sputs(buffer + i, " ");
	i += ft_sputs(buffer + i, msg);
	i += ft_sputs(buffer + i, "\n");
	if (i > number_of_philos * MC)
	{
		write(1, buffer, i);
		i = 0;
		return ;
	}
}
