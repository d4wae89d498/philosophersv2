#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void	*rt(void *d)
{
//	sleep(2);
}

int main()
{
	pthread_t	th;

	pthread_create(&th, 0, &rt, 0);

	pthread_join(th, 0);
}
