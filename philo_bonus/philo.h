/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:11:56 by mfaussur          #+#    #+#             */
/*   Updated: 2023/03/03 22:16:30 by mfaussur         ###   ########lyon.fr   */
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
# include <limits.h>
# include <sys/wait.h>
# include <fcntl.h>
# ifndef LOG
#  define LOG 0
# endif
# ifndef MAX_PROCESS
#  define MAX_PROCESS 84000
# endif

typedef struct s_sems
{
	sem_t	*console;
	sem_t	*forks;
	sem_t	*dead;
	sem_t	*last_meal;
	sem_t	*remaining_eat;
}	t_sems;
typedef struct s_args
{
	long	number_of_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
	t_sems	sems;
}	t_args;
typedef enum e_state
{
	penser,
	manger,
	dormir,
	plus_faim,
}	t_state;
typedef struct s_watcher_args
{
	unsigned long			*last_meal;
	sem_t					*last_meal_sem;
	unsigned long			start_time;
	sem_t					*dead;
	unsigned int			id;
	t_args					args;
	sem_t					*console;
}	t_watcher_args;
long			ft_atol(char *s);
int				ft_strlen(const char *s);
int				ft_puts(const char *s);
int				ft_eputs(const char *s);
void			ultoa(char *o, unsigned long n);
unsigned long	current_time(unsigned long start);
void			ft_sleep(unsigned long time);
void			philo_msg(unsigned long time, unsigned int id, char *msg);
void			msg(sem_t *console, unsigned long start_time, int id,
					char *msg);
int				init_sems(t_sems *sems, long number_of_philos);
void			*watch_famine(void *data);
int				destroy_sems(t_sems *sems);
int				handle_one(t_args args);
void			start_routine(t_args args, t_sems sems, unsigned long start_time,
					long i);
#endif
