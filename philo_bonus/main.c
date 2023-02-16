/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 18:48:33 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


extern int errno;

static long	ft_atol(char *s)
{
	long	n;

	n = 0;
	while (*s >= '0' && *s <= '9')
	{
		n *= 10;
		n += *s++ - '0';
	}
	if (*s)
		return (-1);
	return (n);
}

/*
   void	log_args(t_args args)
   {
   (void) args;
#if LOG
printf("time_to_philos: %li\n", args.number_of_philos);
printf("time_to_die: %li\n", args.time_to_die);
printf("time_to_eat: %li\n", args.time_to_eat);
printf("time_to_sleep: %li\n", args.time_to_sleep);
printf("number_of_meals: %li\n", args.number_of_meals);
#endif
}
*/

void	msg(sem_t *console, unsigned long start_time, int id, char *msg)
{
	sem_wait(console);

	philo_msg((unsigned long)(current_time(start_time) / 1000), id, msg);

	sem_post(console);
}


#include <limits.h>
void	routine(t_args args, pid_t id, sem_t *forks, volatile unsigned long	*last_meal, sem_t *console, unsigned long start_time, sem_t *last_meal_sem)
{
	long			meals;

	if (id % 2)
		ft_sleep(1);
	meals = 0;
	while (1)
	{          
		msg(console, start_time, id, "is thinking");

		sem_wait(forks);
		msg(console, start_time, id, "has taken a fork");
		sem_wait(forks);
		msg(console, start_time, id, "has taken a fork");
		msg(console, start_time, id, "is eating");


		sem_wait(last_meal_sem);
		*last_meal = current_time(start_time);
		sem_post(last_meal_sem);

		ft_sleep(args.time_to_eat);
		meals += 1;
		if (meals >= args.number_of_meals && args.number_of_meals > 0)
		{
			sem_wait(last_meal_sem);
			*last_meal = LONG_MAX;
			sem_post(last_meal_sem);

			sem_post(forks);
			sem_post(forks);
			return ;
		}

		sem_post(forks);
		sem_post(forks);

		msg(console, start_time, id, "is sleeping");
		ft_sleep(args.time_to_sleep);
		

	}
}

typedef struct
{
	volatile unsigned long	*last_meal;
	sem_t					*last_meal_sem;
	unsigned long			start_time;
	sem_t					*dead;

	unsigned int			id;
	t_args					args;
	sem_t 					*console;
	sem_t 					*dead_console;
}	t_watcher_args;

void	*watch(void *data)
{
	t_watcher_args	*watcher_args;

	watcher_args = data;

	while (1)
	{
		ft_sleep(1);
		sem_wait(watcher_args->last_meal_sem);
		int cond = ((long)(current_time(watcher_args->start_time) - *(watcher_args->last_meal))
					> ((long)watcher_args->args.time_to_die * 1000));
		sem_post(watcher_args->last_meal_sem);
		if (cond)
		{
		//	sem_post(watcher_args->console);
			ft_sleep(1);
			msg(watcher_args->dead_console, watcher_args->start_time, watcher_args->id, "died");
			sem_post(watcher_args->dead);
			break ;
		}
	}

	return (0);
}

void	*waite(void *d)
{
	sem_t	*dead;

	dead = d;
	while (waitpid(-1, 0, 0) > -1)
		;
	sem_post(d);
	return (0);
}

int	start(t_args args)
{
	static sem_t					*forks;
	pid_t							pid;
	long							i;

	static sem_t					*dead[MAX_PROCESS];

	pid_t							childs[MAX_PROCESS];

	sem_t 							*console;
	sem_t							*dead_console;
	unsigned long					start_time;
	

	sem_unlink("forks");

	forks = sem_open("forks", O_CREAT, 0644, args.number_of_philos);
	if (forks == SEM_FAILED)
		return (!!printf("Error: sem_open.\n"));


	
	sem_unlink("console");
	console = sem_open("console", O_CREAT, 0644, 1);

	sem_unlink("dead_console");
	dead_console = sem_open("dead_console", O_CREAT, 0644, 1);

	start_time = current_time(0);

	i = -1;
	while (++i < args.number_of_philos)
	{
		char 	s[255];

		s[0] = 'd';
		s[1] = 'c';
		ultoa(s + 2, i);
		sem_unlink(s);
		dead[i] = sem_open(s, O_CREAT, 0644, 0);
		pid = fork();
		if (!pid) 
		{
			pthread_t						watcher;
			sem_t							*last_meal_sem;
		
			s[0] = 'd';
			s[1] = 'd';
			ultoa(s + 2, i);
			sem_unlink(s);

			last_meal_sem = sem_open(s, O_CREAT, 0644, 1);



			static volatile unsigned long	last_meal;	
			t_watcher_args					watcher_args;


			last_meal = start_time;

			watcher_args.last_meal_sem = last_meal_sem;
			watcher_args.start_time = start_time;
			watcher_args.dead = dead[i];
			watcher_args.last_meal = &last_meal;
			watcher_args.id = i + 1;
			watcher_args.args = args;
			watcher_args.console = console;
			watcher_args.dead_console = console;
			pthread_create(&watcher, 0, &watch, &watcher_args); 
			routine(args, i + 1, forks, &last_meal, console, start_time, last_meal_sem);
			sem_post(dead[i]);
			pthread_join(watcher, 0);
			exit(0);
		}
		childs[i] = pid;
	}

	pthread_t	t;
	pthread_create(&t, 0, &waite, dead);


		i = 0;
		while (i < args.number_of_philos)
		{
			sem_wait(dead[i]);
			kill(childs[i++], SIGKILL);
			sem_close(dead[i]);
			char 	s[255];

			s[0] = 'd';
			s[1] = 'c';
			ultoa(s + 2, i);
			sem_unlink(s);
		}


	
	pthread_join(t, 0);

	sem_close(dead_console);
	sem_unlink("dead_console");	
	sem_close(console);
	sem_unlink("console");	
	sem_close(forks);
	sem_unlink("forks");	

	exit(0);

	

	return (0);
}

int	main(int ac, char **av)
{
	static t_args	args;
	int				r;

	if (ac != 5 && ac != 6)
		return (!!printf("Error: invalid arguments.\n"));
	args.number_of_philos = ft_atol(av[1]);
	args.time_to_die = ft_atol(av[2]);
	args.time_to_eat = ft_atol(av[3]);
	args.time_to_sleep = ft_atol(av[4]);
	args.number_of_meals = -1;
	if (ac == 6)
	{
		args.number_of_meals = ft_atol(av[5]);
		if (args.number_of_meals < 0)
			return (!!printf("Error: invalid arguments.\n"));
	}
	if (args.number_of_philos < 0 || args.time_to_die < 0
			|| args.time_to_eat < 0 || args.time_to_sleep < 0)
		return (!!printf("Error: invalid arguments.\n"));
	if (args.number_of_meals == 0)
		return (0);
	r = (start(args));

	return (r);
}
