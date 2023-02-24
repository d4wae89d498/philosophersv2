#ifndef SMART_MUTEX_H
# define SMART_MUTEX_H
# include <pthread.h>

typedef struct s_mtx
{
	pthread_mutex_t		value_mtx;
	pthread_mutex_t		lock_mtx;
	int					locked;
}	t_mtx;

int	mtx_init(t_mtx *mtx);
int	mtx_destroy(t_mtx *mtx);
int	mtx_lock(t_mtx *mtx);
int	mtx_unlock(t_mtx *mtx);
int	mtx_is_locked(t_mtx *mtx);
#endif
