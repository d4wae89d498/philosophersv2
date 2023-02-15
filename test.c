#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    sem_t *sem = sem_open("/my_semaphore", O_CREAT | O_EXCL, 0666, 0);
    if (sem == SEM_FAILED) {
        perror("sem_open failed");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // child process
        int value = 42;
        printf("Child: sending value %d\n", value);
        sem_post(sem);  // signal parent process
        sem_wait(sem);  // wait for parent to acknowledge
        sem_close(sem);
        exit(EXIT_SUCCESS);
    } else {  // parent process
        sem_wait(sem);  // wait for child to send signal
        int value;
        sem_post(sem);  // acknowledge signal
		printf("%d\n", value);
        sem_close(sem);
        sem_unlink("/my_semaphore");
        wait(NULL);
    }

    return 0;
}

