/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 12:33:48 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:33:49 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	handle_one(t_args args)
{
	printf("%d 1 is thinking\n", 0);
	printf("%d 1 has taken a fork\n", 0);
	ft_sleep(args.time_to_die);
	printf("%lu 1 died\n", args.time_to_die);
	return (0);
}
