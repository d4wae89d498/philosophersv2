#include <stdio.h>
#include <stdlib.h>

unsigned long	current_time(unsigned long start)
{
	struct timeval 					time;

	gettimeofday(&time, 0);
	return ((time.tv_sec * 1000000 + time.tv_usec) - start);
}

int main()
{
	printf("%lu\n", current_time(0));
	usleep(1);
	printf("%lu\n", current_time(0));
}
