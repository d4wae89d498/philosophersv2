/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 06:51:40 by mfaussur          #+#    #+#             */
/*   Updated: 2023/04/02 15:24:03 by mafaussu         ###   ########.fr       */
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
#  define MAX_THREADS 200
# endif
# ifndef MC
#  define MC 0
# endif
# ifndef WATCHER_SLEEP
#  define WATCHER_SLEEP 5555
# endif
# ifndef THINK_MIN_DELAY
#  define THINK_MIN_DELAY 222
# endif
# ifndef START_DELAY
#  define START_DELAY 0
# endif
# ifndef START_MUL
#  define START_MUL 0
# endif
# ifndef SLEEP_CHECK_DELAY
#  define SLEEP_CHECK_DELAY 5000
# endif
# ifndef SLEEP_CHECK_TICK
#  define SLEEP_CHECK_TICK 4
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
	long					number_of_philos;
	long					time_to_die;
	long					time_to_eat;
	long					time_to_sleep;
	long					number_of_meals;
	unsigned long			start;
}	t_args;
typedef struct s_philo_ctx
{
	pthread_mutex_t			*dead_mtx;
	pthread_mutex_t			*state_mtx;
	int						*dead;
	unsigned int			id;
	pthread_mutex_t			*left_fork;
	pthread_mutex_t			*right_fork;
	pthread_mutex_t			*console_mtx;
	t_args					args;
	t_state					state;	
	unsigned long			last_eat_time;
	long					meals;

	unsigned long			start;
}	t_philo_ctx;
typedef struct s_watcher_args
{
	pthread_t				*philos;
	t_philo_ctx				*philo_ctx;
	t_args					args;
	int						dead;
}	t_watcher_args;
typedef struct s_dinning_simulation
{
	t_args					args;
	t_philo_ctx				philo_ctx[MAX_THREADS];
	pthread_t				philo_thread[MAX_THREADS];
	pthread_mutex_t			fork_mtx[MAX_THREADS];
	pthread_mutex_t			state_mtx[MAX_THREADS];
	pthread_mutex_t			console_mtx;
	pthread_mutex_t			dead_mtx;
}	t_dinning_simulation;

int				ft_sputs(char *dst, const char *src);
int				ft_eputs(char *str);
int				ft_puts(char *str);
int				ultoa(char *o, unsigned long n);
long			ft_atol(char *s);
int				ft_strlen(const char *s);
int				destroy_mutex(pthread_mutex_t *mtx);
void			*philo_routine(void *data);
int				msg(t_philo_ctx *ctx, t_state state);
void			assign_forks(t_dinning_simulation *sim);
int				init_philo_ctx(t_dinning_simulation *sim);
int				destroy_philo_ctx(t_dinning_simulation *sim);
int				start_simulation(t_args args);
int				start_philos(t_dinning_simulation *sim);
int				start_watcher(t_dinning_simulation *sim);
unsigned long	current_time(unsigned long start);
void			ft_usleep(unsigned long time);
void			ft_sleep(unsigned long time);
void			philo_msg(long number_of_philos, unsigned long time,
					unsigned int id, char *msg);
int				sleep_while_check_dead(t_philo_ctx *ctx, unsigned long time);
int				handle_one(t_args args);
#endif
