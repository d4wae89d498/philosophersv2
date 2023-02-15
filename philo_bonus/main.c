/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfaussur <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 21:14:18 by mfaussur          #+#    #+#             */
/*   Updated: 2023/02/15 13:27:07 by mfaussur         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include "string.h"

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

void	msg(sem_t *console, int id, char *msg)
{
	sem_wait(console);

	printf("%lu philo %i %s\n", (unsigned long)(current_time() / 1000), id, msg);
fflush(stdout); 


	sem_post(console);
}


#include <limits.h>
void	routine(t_args args, pid_t id, sem_t *forks, volatile unsigned long	*last_meal, sem_t *console)
{
	long			meals;

//	if (id % 2)
//		ft_sleep(7);
	meals = 0;
	while (1)
	{          
		msg(console, id, "is thinking");

		sem_wait(forks);
		msg(console, id, "has taken a fork");
		sem_wait(forks);
		msg(console, id, "has taken a fork");
		msg(console, id, "is eating");



		*last_meal = current_time();
		meals += 1;
		if (meals >= args.number_of_meals && args.number_of_meals > 0)
			return ;
		ft_sleep(args.time_to_eat);
		sem_post(forks);
		sem_post(forks);

		msg(console, id, "is sleeping");
		ft_sleep(args.time_to_sleep);
		

	}
}

typedef struct
{
	sem_t					*dead;
	volatile unsigned long	*last_meal;
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
		int cond = ((current_time() - (unsigned long)*(watcher_args->last_meal))
					> ((unsigned long)watcher_args->args.time_to_die * 1000));
		if (cond)
		{

			sem_post(watcher_args->console);
			ft_sleep(4);
			sem_post(watcher_args->dead);
			msg(watcher_args->dead_console, watcher_args->id, "died");


			break ;
		}
	}
	return (0);
}

void	*waite(void *d)
{

	(void) d;
	while (waitpid(-1, 0, 0) > -1)
		;
	return (0);
}

int	start(t_args args)
{
	static sem_t					*forks;
	pid_t							pid;
	long							i;

	static sem_t					*dead;

	pid_t							childs[MAX_PROCESS];

	sem_t 							*console;
	sem_t							*dead_console;

	dead = 0;
	sem_unlink("forks");

	forks = sem_open("forks", O_CREAT, 0644, args.number_of_philos);
	if (forks == SEM_FAILED)
		return (!!printf("Error: sem_open.\n"));
	
	sem_unlink("dead");
	dead = sem_open("dead", O_CREAT, 0644, 0);
	if (forks == SEM_FAILED)
	{
		sem_close(forks);
		sem_unlink("forks");	
		return (!!printf("Error: sem_open.\n"));
	}

	
	sem_unlink("console");
	console = sem_open("console", O_CREAT, 0644, 1);

	sem_unlink("dead_console");
	dead_console = sem_open("dead_console", O_CREAT, 0644, 1);

	i = -1;
	while (++i < args.number_of_philos)
	{
		pid = fork();
		if (!pid) 
		{
			pthread_t						watcher;
			
			static volatile unsigned long	last_meal;	
			t_watcher_args					watcher_args;


			last_meal = current_time();

			watcher_args.dead = dead;
			watcher_args.last_meal = &last_meal;
			watcher_args.id = i + 1;
			watcher_args.args = args;
			watcher_args.console = console;
			watcher_args.dead_console = console;

			pthread_create(&watcher, 0, &watch, &watcher_args); 
			routine(args, i + 1, forks, &last_meal, console);
			pthread_join(watcher, 0);
		
			exit(0);
		}
		childs[i] = pid;
	}

	pthread_t	t;
	pthread_create(&t, 0, &waite, 0);

		sem_wait(dead);
		i = 0;
		while (i < args.number_of_philos)
			kill(childs[i++], SIGKILL);
	
	pthread_join(t, 0);

	sem_close(dead_console);
	sem_unlink("dead_console");	
	sem_close(console);
	sem_unlink("console");	
	sem_close(dead);
	sem_unlink("dead");	
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
