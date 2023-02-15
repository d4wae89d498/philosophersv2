#include <semaphore.h>

int main()
{
	sem_t *sem;

	sem_unlink("/testsem");
	sem = sem_open("/testsem", O_CREAT, 0644, 0);

	sem_wait(sem);

	printf("ok!\n");
}
