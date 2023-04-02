/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafaussu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 11:17:47 by mafaussu          #+#    #+#             */
/*   Updated: 2023/04/02 16:07:48 by mafaussu         ###   ########.fr       */
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
			msg(watcher_args->console, watcher_args->start_time,
				watcher_args->id, DIE);
			sem_post(watcher_args->dead);
			break ;
		}
	}
	return (0);
}
