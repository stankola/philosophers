/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:17:38 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:41:32 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pthread.h>
#include "philo.h"

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	phrint(SLEEP, phil);
	phleep(phil, phil->tts);
}

void	think(t_philosopher *phil)
{
	int	time_to_meal;

	phrint(THINK, phil);
	time_to_meal = get_time_to_meal(phil);
	if (time_to_meal > 0)
		phleep(phil, time_to_meal);
}

void	eat(t_philosopher *phil)
{
	if ((phil->id % 2) && take_fork(phil, phil->r_utensil))
		if (! take_fork(phil, phil->l_utensil))
			drop_fork(phil->r_utensil);
	if (!(phil->id % 2) && take_fork(phil, phil->l_utensil))
		if (! take_fork(phil, phil->r_utensil))
			drop_fork(phil->l_utensil);
	if (!phil->dead)
	{
		if (! should_die(phil))
		{
			phil->prev_meal = get_time_in_us();
			phrint(EAT, phil);
			phleep(phil, phil->tte);
		}
		drop_fork(phil->r_utensil);
		drop_fork(phil->l_utensil);
		phil->eat_count++;
	}
}

int	take_fork(t_philosopher *phil, t_fork *f)
{
	while (! should_die(phil))
	{
		pthread_mutex_lock(&(f->fork_mutex));
		if (f->taken == 0)
		{
			f->taken = 1;
			phrint(FORK_TAKE, phil);
			pthread_mutex_unlock(&(f->fork_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(f->fork_mutex));
		usleep(SNOOZE);
	}
	return (0);
}

inline void	drop_fork(t_fork *f)
{
	pthread_mutex_lock(&(f->fork_mutex));
	f->taken = 0;
	pthread_mutex_unlock(&(f->fork_mutex));
}
