#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define NUM_PHILOSOPHERS 5
#define NUM_FORKS 5

sem_t *forks;

int main() {
    forks = sem_open("/forks", O_CREAT, 0644, NUM_FORKS);

    pid_t pid;
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pid = fork();
        if (pid == 0) {
            while (1) {
                
				printf("is thinking!\n");


                sem_wait(forks);
                sem_wait(forks);

                sleep(1);
				printf("is eating!\n");

                sem_post(forks);
                sem_post(forks);
            }
            exit(0);
        }
    }


    sem_unlink("/forks");
    return 0;
}

