/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 06:51:40 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 06:53:42 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# ifndef LOG
#  define LOG 0
# endif
# ifndef MAX_THREADS
#  define MAX_THREADS 42000
# endif
# include "stdlib.h"
# include "unistd.h"
# include "sys/time.h"
# include <limits.h>

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
typedef struct s_mutexes
{
	pthread_mutex_t	console;
	pthread_mutex_t	dead_console;
	pthread_mutex_t	gdead_mtx;
}	t_mutexes;
typedef struct s_philo_ctx
{
	unsigned int			id;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			*console;
	pthread_mutex_t			*dead_console;
	t_args					args;
	volatile t_state		state;	
	volatile unsigned long	last_eat_time;
	volatile long			meals;
	volatile int			dead;
	volatile int			*gdead;
	pthread_mutex_t			state_mtx;
	pthread_mutex_t			*gdead_mtx;
	unsigned long			start;
}	t_philo_ctx;
typedef struct s_watcher_args
{
	pthread_t	*philos;
	t_philo_ctx	*philos_ctx;
	t_args		args;
}	t_watcher_args;
int				ft_puts(char *str);
void			ultoa(char *o, unsigned long n);
long			ft_atol(char *s);
int				ft_strlen(const char *s);
void			*philo_routine(void *data);
void			dead_msg(t_philo_ctx *ctx, char *msg);
void			msg(t_philo_ctx *ctx, char *msg);
int				init_table(pthread_mutex_t *table, long number_of_philos);
void			init_philos_ctx(t_args args, pthread_mutex_t *table,
					t_philo_ctx *philos_ctx, t_mutexes *m);
int				init_philos(long number_of_philos, t_philo_ctx *philos_ctx,
					pthread_t *philos);
void			*watch_philos(void *data);
unsigned long	current_time(unsigned long start);
void			ft_sleep(unsigned long time);
void			philo_msg(unsigned long time, unsigned int id, char *msg);

#endif
