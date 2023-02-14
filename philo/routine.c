#include "philo.h"

int	try_eat(t_philo_ctx *ctx)
{
	pthread_mutex_lock(ctx->left_fork);
	if (ctx->dead)
		return (1);
	msg(ctx, "has taken a fork");
	pthread_mutex_lock(ctx->right_fork);
	if (ctx->dead)
		return (1);
	ctx->state = manger; 
	ctx->last_eat_time = current_time();
	ctx->meals += 1;
	msg(ctx, "has taken a fork");
	msg(ctx, "is eating");
	ft_sleep(ctx->args.time_to_eat);
	if (ctx->dead)
		return (1);
	pthread_mutex_unlock(ctx->right_fork);
	pthread_mutex_unlock(ctx->left_fork);
	return (0);
}

void	*philo_routine(void *data)
{
	t_philo_ctx	*ctx;

	ctx = data;
	if (ctx->id % 2)
		ft_sleep(1);
	while (ctx->meals < ctx->args.number_of_meals
			|| (ctx->args.number_of_meals < 0))
	{
		if (ctx->dead)
			return ((void*)1);
		if (try_eat(ctx))
			return ((void*)1);
		ctx->state = dormir;
		msg(ctx, "is sleeping");
		ft_sleep(ctx->args.time_to_sleep);
		if (ctx->dead)
			return ((void*)1);
		ctx->state = penser;	
		msg(ctx, "is thinking");
	}
	ctx->state = plus_faim;
	return (0);
}
