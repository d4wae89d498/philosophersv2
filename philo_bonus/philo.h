/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:11:56 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 18:21:53 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <semaphore.h>
# include <signal.h>
# include <sys/time.h>
# include <pthread.h>
# ifndef LOG
#  define LOG 0
# endif
# ifndef MAX_PROCESS
#  define MAX_PROCESS 84000
# endif

typedef struct s_args
{
	long	number_of_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_args;
typedef enum e_state
{
	penser,
	manger,
	dormir,
	plus_faim,
}	t_state;
unsigned long	current_time(unsigned long start);
void			ft_sleep(unsigned long time);
void			philo_msg(unsigned long time, unsigned int id, char *msg);
void			ultoa(char *o, unsigned long n);
#endif
