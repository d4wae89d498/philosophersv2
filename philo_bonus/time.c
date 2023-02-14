#include "philo.h"

unsigned long	current_time()
{
	struct timeval 	time;
	static unsigned long	start;

	gettimeofday(&time, 0);
	if (start == 0)
		start = ((time.tv_sec * 1000000 + time.tv_usec));
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

void	ft_sleep(unsigned long time)
{
	unsigned long	start;

	time *= 1000;
	start = current_time();


	usleep(time * 0.95);
	while (current_time() < start + time)
		usleep(42);
}
