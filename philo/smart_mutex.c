#include "smart_mutex.h"

int	mtx_init(t_mtx *mtx)
{
	if (pthread_mutex_init(&(mtx->value_mtx), 0))
		return (1);
	if (pthread_mutex_init(&(mtx->lock_mtx), 0))
	{
		pthread_mutex_destroy(&(mtx->value_mtx));
		return (1);
	}
	mtx->locked = 0;
	return (0);
}

int	mtx_lock(t_mtx *mtx)
{
	pthread_mutex_lock(&(mtx->lock_mtx));
	mtx->locked = 1;
	pthread_mutex_unlock(&(mtx->lock_mtx));
	return (pthread_mutex_lock(&(mtx->value_mtx)));
}

int	mtx_unlock(t_mtx *mtx)
{
	int	r;

	r = pthread_mutex_unlock(&(mtx->value_mtx));
	pthread_mutex_lock(&(mtx->lock_mtx));
	mtx->locked = 0;
	pthread_mutex_unlock(&(mtx->lock_mtx));
	return (r);
}

int	mtx_destroy(t_mtx *mtx)
{
	if (mtx_is_locked(mtx))
		mtx_unlock(mtx);
	return (!!(pthread_mutex_destroy(&(mtx->value_mtx))
			+ pthread_mutex_destroy(&(mtx->lock_mtx))));
}

int	mtx_is_locked(t_mtx *mtx)
{
	int	r;

	pthread_mutex_lock(&(mtx->lock_mtx));
	r = mtx->locked;
	pthread_mutex_unlock(&(mtx->lock_mtx));
	return (r);
}
