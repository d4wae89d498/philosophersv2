#include "philo.h"

int main()
{
    	char    		s[255];
	unsigned long	ul;
//    printf("%i, %s\n", ultoa(s, 102), s);

	printf("-%lu\n", current_time(0));
	usleep(0);
	printf("-%lu\n", current_time(0));

	ul = current_time(0);
	ft_sleep(200);
	printf("-%lu\n", current_time(0) - ul);
}
