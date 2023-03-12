#include "super_mutex.h"

int super_mutex_init(t_super_mutex *mutex)
{
    mutex->is_locked = 0;
    return (pthread_mutex_init(&mutex->lock, NULL)
    + pthread_mutex_init(&mutex->is_locked_lock, NULL));
}

void super_mutex_destroy(t_super_mutex *mutex)
{
    int is_locked;
    pthread_mutex_lock(&mutex->is_locked_lock);
    is_locked = mutex->is_locked;
    mutex->is_locked = 0;
    pthread_mutex_unlock(&mutex->is_locked_lock);

    if (is_locked)
    {
        pthread_mutex_unlock(&mutex->lock);
    }

    pthread_mutex_destroy(&mutex->lock);
    pthread_mutex_destroy(&mutex->is_locked_lock);
}

void super_mutex_lock(t_super_mutex *mutex)
{
    pthread_mutex_lock(&mutex->is_locked_lock);
    mutex->is_locked = 1;
    pthread_mutex_unlock(&mutex->is_locked_lock);
    pthread_mutex_lock(&mutex->lock);
}

void super_mutex_unlock(t_super_mutex *mutex)
{
    int is_locked;
    pthread_mutex_lock(&mutex->is_locked_lock);
    is_locked = mutex->is_locked;
    if (is_locked)
    {
        mutex->is_locked = 0;
        pthread_mutex_unlock(&mutex->lock);
    }
    pthread_mutex_unlock(&mutex->is_locked_lock);
}

int super_mutex_is_locked(t_super_mutex *mutex)
{
    int is_locked;
    pthread_mutex_lock(&mutex->is_locked_lock);
    is_locked = mutex->is_locked;
    pthread_mutex_unlock(&mutex->is_locked_lock);
    return is_locked;
}