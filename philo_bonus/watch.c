/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:47 by mafaussu          #+#    #+#             */
/*   Updated: 2023/03/03 22:18:57 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*watch_famine(void *data)
{
	t_watcher_args	*watcher_args;
	int				cond;

	watcher_args = data;
	while (1)
	{
		ft_sleep(5);
		sem_wait(watcher_args->last_meal_sem);
		cond = ((long)(current_time(watcher_args->start_time)
					- *(watcher_args->last_meal))
				> ((long)watcher_args->args.time_to_die * 1000));
	
		sem_post(watcher_args->last_meal_sem);
		if (cond)
		{
			sem_wait(watcher_args->console);
			msg(0, watcher_args->start_time, watcher_args->id, "died");
			sem_post(watcher_args->dead);
			break ;
		}

	}
	return (0);
}
