/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   puts.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 15:41:32 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/24 20:29:47 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

int	ft_dputs(int fd, char *str)
{
	int	len;

	len = ft_strlen(str);
	if (write(fd, str, len) < 0)
	{
	}
	return (len);
}

int	ft_eputs(char *str)
{
	return (ft_dputs(2, str));
}

int	ft_puts(char *str)
{
	return (ft_dputs(1, str));
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

int	ft_strlen(const char *s)
{
	int	i;

	i = 0;
	while (*s++)
		i += 1;
	return (i);
}
