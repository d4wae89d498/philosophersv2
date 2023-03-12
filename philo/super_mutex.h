#ifndef SUPER_MUTEX_H
#define SUPER_MUTEX_H

#include <pthread.h>

typedef struct {
    pthread_mutex_t lock;
    pthread_mutex_t is_locked_lock;
    int is_locked;
    int is_lock_unlocked;
} t_super_mutex;

int super_mutex_init(t_super_mutex *mutex);
void super_mutex_destroy(t_super_mutex *mutex);
void super_mutex_lock(t_super_mutex *mutex);
void super_mutex_unlock(t_super_mutex *mutex);
int super_mutex_is_locked(t_super_mutex *mutex);

#endif /* SUPER_MUTEX_H */
