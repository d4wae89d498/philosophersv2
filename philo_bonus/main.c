/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/16 23:01:14 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	msg(sem_t *console, unsigned long start_time, int id, char *msg)
{
	if (console)
		sem_wait(console);
	philo_msg((unsigned long)(current_time(start_time) / 1000), id, msg);
	if (console)
		sem_post(console);
}

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
			/*
			ft_sleep(1);
			msg(watcher_args->dead_console, watcher_args->start_time, watcher_args->id, "died");
			sem_post(watcher_args->dead);
			*/

			
			sem_wait(watcher_args->console);
			
		//	sem_post(watcher_args->dead);
			msg(0, watcher_args->start_time, watcher_args->id, "died");

			sem_post(watcher_args->dead);
			//ft_sleep(1);
			//sem_post(watcher_args->console);
			
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
	t_sems			sems;
	pid_t			pid;
	long			i;
	unsigned long	start_time;
	pid_t			childs[MAX_PROCESS];	

	sem_unlink("forks");

	sems.forks = sem_open("forks", O_CREAT, 0644, args.number_of_philos);
	if (sems.forks == SEM_FAILED)
		return (!!ft_puts("Error: sem_open.\n"));


	
	sem_unlink("console");
	sems.console = sem_open("console", O_CREAT, 0644, 1);

	sem_unlink("dead_console");
	sems.dead_console = sem_open("dead_console", O_CREAT, 0644, 1);

	start_time = current_time(0);

	const char *dead_sem_label = "dead";
	sem_unlink(dead_sem_label);
	sems.dead = sem_open(dead_sem_label, O_CREAT, 0644, 0);

	i = -1;
	while (++i < args.number_of_philos)
	{
		char 	dead_sem_label[255];

		dead_sem_label[0] = 'd';
		dead_sem_label[1] = 'c';
		ultoa(dead_sem_label + 2, i);
		sem_unlink(dead_sem_label);

		pid = fork();
		if (!pid) 
		{
			pthread_t						watcher;
			sem_t							*last_meal_sem;
			static volatile unsigned long	last_meal;	
			t_watcher_args					watcher_args;
			char							last_meal_sem_label[255];

			last_meal_sem_label[0] = 'd';
			last_meal_sem_label[1] = 'd';
			ultoa(last_meal_sem_label + 2, i);
			sem_unlink(last_meal_sem_label);


			last_meal_sem = sem_open(last_meal_sem_label, O_CREAT, 0644, 1);






			last_meal = start_time;

			watcher_args.last_meal_sem = last_meal_sem;
			watcher_args.start_time = start_time;
			watcher_args.dead = sems.dead;
			watcher_args.last_meal = &last_meal;
			watcher_args.id = i + 1;
			watcher_args.args = args;
			watcher_args.console = sems.console;
			watcher_args.dead_console = sems.dead_console;
			pthread_create(&watcher, 0, &watch, &watcher_args); 
			routine(args, i + 1, sems.forks, &last_meal, sems.console, start_time, last_meal_sem);
			sem_post(sems.dead);
			pthread_join(watcher, 0);
		
			sem_close(last_meal_sem);
			sem_unlink(last_meal_sem_label);

			exit(0);
		}
		childs[i] = pid;
	}

	pthread_t	t;
	pthread_create(&t, 0, &waite, sems.dead);


		sem_wait(sems.dead);
		i = 0;
		while (i < args.number_of_philos)
		{
			kill(childs[i], SIGKILL);
			char 	s[255];
			s[0] = 'd';
			s[1] = 'd';
			ultoa(s + 2, i);
			sem_unlink(s);
			i += 1;
		}


	
	pthread_join(t, 0);

	sem_close(sems.dead_console);
	sem_unlink("dead_console");	
	sem_close(sems.console);
	sem_unlink("console");	
	sem_close(sems.forks);
	sem_unlink("forks");	

	exit(0);

	

	return (0);
}

int	main(int ac, char **av)
{
	static t_args	args;

	if (ac != 5 && ac != 6)
		return (!!ft_puts("Error: invalid arguments.\n"));
	args.number_of_philos = ft_atol(av[1]);
	args.time_to_die = ft_atol(av[2]);
	args.time_to_eat = ft_atol(av[3]);
	args.time_to_sleep = ft_atol(av[4]);
	args.number_of_meals = -1;
	if (ac == 6)
	{
		args.number_of_meals = ft_atol(av[5]);
		if (args.number_of_meals < 0)
			return (!!ft_puts("Error: invalid arguments.\n"));
		else if (args.number_of_meals == 0)
			return (0);
	}
	if (args.number_of_philos < 0 || args.time_to_die < 0
			|| args.time_to_eat < 0 || args.time_to_sleep < 0)
		return (!!ft_puts("Error: invalid arguments.\n"));
	return (start(args));
}
