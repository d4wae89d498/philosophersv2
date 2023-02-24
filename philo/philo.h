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
# include "smart_mutex.h"
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
	long			number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_meals;
	unsigned long	start;
}	t_args;
typedef enum e_state
{
	penser,
	manger,
	dormir,
	plus_faim,
}	t_state;
typedef struct s_philo_ctx
{
	pthread_mutex_t			*dead_mtx;
	int						*dead;
	unsigned int			id;
	t_mtx					*left_fork;
	t_mtx					*right_fork;
	pthread_mutex_t			*console;
	t_args					args;
	volatile t_state		state;	
	volatile unsigned long	last_eat_time;
	volatile long			meals;
	pthread_mutex_t			state_mtx;
	unsigned long			start;
}	t_philo_ctx;
typedef struct s_watcher_args
{
	pthread_t	*philos;
	t_philo_ctx	*philos_ctx;
	t_args		args;
	int			dead;
}	t_watcher_args;
int				destroy_philos_ctx(t_philo_ctx *philos_ctx,
					long number_of_philos);
int				ft_puts(char *str);
int				ultoa(char *o, unsigned long n);
long			ft_atol(char *s);
int				ft_strlen(const char *s);
void			*philo_routine(void *data);
int				msg(t_philo_ctx *ctx, char *msg);
int				init_table(t_mtx *table, long number_of_philos);
int				init_philos_ctx(t_args args, t_mtx *table,
					t_philo_ctx *philos_ctx, pthread_mutex_t *console);
int				init_philos(long number_of_philos, t_philo_ctx *philos_ctx,
					pthread_t *philos);
void			*watch_philos(void *data);
unsigned long	current_time(unsigned long start);
void			ft_sleep(unsigned long time);
void			philo_msg(long number_of_philos, unsigned long time,
					unsigned int id, char *msg);
int				sleep_while_check_dead(t_philo_ctx *ctx, unsigned long time);

#endif
