#include "philo.h"

#include <errno.h>          /* errno, ECHILD            */
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

void	msg(int id, char *msg)
{
		printf("%lu philo %i %s\n", (unsigned long)(current_time() / 1000), id, msg);
}

void	routine(t_args args, pid_t id, sem_t *forks, volatile int	*dead)
{
	unsigned long	last_meal;
	long			meals;

	if (id % 2)
		ft_sleep(7);
	last_meal = 0;
	meals = 0;
	while (!*dead)
	{          
		if (current_time() - (unsigned long)last_meal > (unsigned long)args.time_to_die * 1000)
		{
			*dead = 1;
			msg(id, "died");
		}
		msg(id, "is thinking");
		sem_wait(forks);
		msg(id, "has taken a fork");
		sem_wait(forks);
		msg(id, "has taken a fork");
		msg(id, "is eating");
		meals += 1;
		if (meals >= args.number_of_meals && args.number_of_meals > 0 && (++*dead))
			return ;
		last_meal = current_time();	
		ft_sleep(args.time_to_eat);
		sem_post(forks);
		sem_post(forks);
		msg(id, "is sleeping");
		if (current_time() - (unsigned long)last_meal + (unsigned long)args.time_to_sleep * 1000 > (unsigned long)args.time_to_die * 1000)
		{
			*dead = 1;
			msg(id, "died");
		}
		ft_sleep(args.time_to_sleep);
	}
}

#include <signal.h>

int	start(t_args args)
{
	sem_t			*forks;
	pid_t			pid;
	long			i;	
	volatile int	dead;

	dead = 0;
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, 0644, args.number_of_philos);
	if (forks == SEM_FAILED)
		return (!!printf("Error: sem_open.\n"));
	i = 0;
	while (i < args.number_of_philos)
	{
		pid = fork();
		if (!pid) 
		{
			routine(args, i + 1, forks, &dead);
			sem_post(forks);
			sem_post(forks);
			exit(0);
		}

		i += 1;
	}
	while (waitpid(-1, 0, 0) > -1)
		;
	sem_close(forks);
	sem_unlink("forks");	
	return (0);
}

int main(int ac, char **av)
{
	t_args			args;

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
	log_args(args);
	return (start(args));
}
