#include "philo.h"

int	destroy_sems(t_sems *sems)
{
	int	r;

	r = 0;
	if (sems->forks)
	{
		r += sem_close(sems->forks);
		sems->forks = 0;
	}
	if (sems->dead)
	{
		r += sem_close(sems->dead);
		sems->dead = 0;
	}
	if (sems->console)
	{
		r += sem_close(sems->console);
		sems->console = 0;
	}
	r += sem_unlink("dead");
	r += sem_unlink("console");
	r += sem_unlink("forks");
	return (r);
}

int	init_sems(t_sems *sems, long number_of_philos)
{
	*sems = (t_sems){0, 0, 0, 0};
	destroy_sems(sems);
	sems->forks = sem_open("forks", O_CREAT, 0644, number_of_philos);
	if (sems->forks == SEM_FAILED)
		return (!!(ft_puts("Error: sem_open.\n") + destroy_sems(sems)));
	sems->console = sem_open("console", O_CREAT, 0644, 1);
	if (sems->console == SEM_FAILED)
		return (!!(ft_puts("Error: sem_open.\n") + destroy_sems(sems)));
	sems->dead = sem_open("dead", O_CREAT, 0644, 0);
	if (sems->console == SEM_FAILED)
		return (!!(ft_puts("Error: sem_open.\n") + destroy_sems(sems)));
	return (0);
}
