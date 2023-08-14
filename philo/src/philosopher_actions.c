/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:17:38 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/05 00:59:50 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include "philo.h"

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	phrint(SLEEP, phil);
	phleep(phil, phil->tts);
}

#include <stdio.h>
void	think(t_philosopher *phil)
{
	int meal_slots = 2 + phil->no_of_phils % 2;
	int simulation_time = get_time_in_us() - (phil->inception * 1000);
	int meal_duration = phil->ttd / meal_slots - 25 * ((phil->ttd - meal_slots * phil->tte) / 100);
	int meal_cycle_time = simulation_time % (meal_slots * meal_duration);
	int current_meal_time_slot = meal_cycle_time / meal_duration;
	int time_to_meal;

	phrint(THINK, phil);
//	fprintf(stderr, "%ld %d meal_duration %d meal_cycle_time %d current_meal_time_slot %d time to live %ld\n", get_time_in_ms() - phil->inception, phil->id, meal_duration, meal_cycle_time, current_meal_time_slot, phil->prev_meal + phil->ttd - get_time_in_us());
	if (phil->no_of_phils % 2 && phil->id == 1)		// 3. slot, 1 phil when odd phils
	{
		if (current_meal_time_slot == 2)
			return ;
		time_to_meal = 2 * meal_duration - meal_cycle_time;
	}
	else if (phil->id % 2)	// 1. slot, odd phil
	{
		if (current_meal_time_slot == 0)
			return ;
		time_to_meal = meal_slots * meal_duration - meal_cycle_time;
	}
	else					// 2. slot, even phil
	{
		if (current_meal_time_slot == 1)
			return ;
		time_to_meal = (meal_cycle_time < meal_duration) * (meal_duration - meal_cycle_time) + (meal_cycle_time >= 2 * meal_duration) * (meal_duration + 3 * meal_duration - meal_cycle_time);
	}
//	fprintf(stderr, "%ld %d time to meal %d\n", get_time_in_ms() - phil->inception, phil->id, time_to_meal);
	phleep(phil, time_to_meal);
}

void	eat(t_philosopher *phil){
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
		pthread_mutex_lock(&(f->grab_mutex));
		if (should_die(phil))
		{
			pthread_mutex_unlock(&(f->grab_mutex));
			return (0);
		}
		if (f->taken == 0)
		{
			f->taken = 1;
			pthread_mutex_lock(&(f->fork_mutex));
			phrint(FORK_TAKE, phil);
			pthread_mutex_unlock(&(f->grab_mutex));
			return (1);
		}
		pthread_mutex_unlock(&(f->grab_mutex));
		usleep(SHORT_SLEEP_INTERVAL);		// TODO Meal slot check here?
	}
	return (0);
}

inline void	drop_fork(t_fork *f)
{
	pthread_mutex_unlock(&f->fork_mutex);
	pthread_mutex_lock(&(f->grab_mutex));
	f->taken = 0;
	pthread_mutex_unlock(&(f->grab_mutex));
}
