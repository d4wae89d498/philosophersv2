/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 06:51:40 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/25 12:07:32 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <limits.h>
# ifndef MAX_THREADS
#  define MAX_THREADS 420
# endif
# ifndef MC
#  define MC 50
# endif
# ifndef WATCHER_SLEEP
#  define WATCHER_SLEEP 4444
# endif
# ifndef START_DELAY
#  define START_DELAY 1000
# endif
# ifndef MIN_SLEEP_DELAY
#  define MIN_SLEEP_DELAY 100000
# endif
# ifndef SLEEP_CHECK_DELAY
#  define SLEEP_CHECK_DELAY 5000
# endif
# ifndef SLEEP_CHECK_TICK
#  define SLEEP_CHECK_TICK 2000
# endif

typedef enum e_state
{
	TAKE,
	EAT,
	THINK,
	SLEEP,
	DIE,
	END
}	t_state;

typedef struct s_args
{
	long			number_of_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			number_of_meals;
	unsigned long	start;
}	t_args;
typedef struct s_philo_ctx
{
	pthread_mutex_t			*dead_mtx;
	int						*dead;
	unsigned int			id;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
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
int				destroy_mutex(pthread_mutex_t *mtx);
int			start_watcher(t_args args, pthread_t *philos,
					t_philo_ctx *philos_ctx);
int				destroy_philos_ctx(t_philo_ctx *philos_ctx,
					long number_of_philos);
int				ft_sputs(char *dst, const char *src);
int				ft_eputs(char *str);
int				ft_puts(char *str);
int				ultoa(char *o, unsigned long n);
long			ft_atol(char *s);
int				ft_strlen(const char *s);
void			*philo_routine(void *data);
int				msg(t_philo_ctx *ctx, t_state state);
int				init_table(pthread_mutex_t *table, long number_of_philos);
int				init_philos_ctx(t_args args, pthread_mutex_t *table,
					t_philo_ctx *philos_ctx, pthread_mutex_t *console);
int				init_philos(long number_of_philos, t_philo_ctx *philos_ctx,
					pthread_t *philos);
unsigned long	current_time(unsigned long start);
void			ft_usleep(unsigned long time);
void			ft_sleep(unsigned long time);
void			philo_msg(long number_of_philos, unsigned long time,
					unsigned int id, char *msg);
int				sleep_while_check_dead(t_philo_ctx *ctx, unsigned long time);
#endif
