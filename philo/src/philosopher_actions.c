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
	long int	sleep_start;

	if (! should_die(phil))
	{
		sleep_start = get_time_in_us();
		phrint(SLEEP, phil);
		while (!should_die(phil)
			&& (get_time_in_us() - phil->tts) < sleep_start)
			usleep(SLEEP_CYCLE);
//		{
//			if (phil->id == 1)
//				usleep(SLEEP_CYCLE * 2 / 5);
//			else
//				usleep(SLEEP_CYCLE * (phil->id % 2));
//		}
	}
}

#include <stdio.h>
void	think(t_philosopher *phil)
{	// This seems to work fairly well with odd number of phils. On even number it starts to desync off the bat
	int	sync_time;

	phrint(THINK, phil);
//	usleep(random() % 1000);
//	if (phil->id == 1)
//		usleep(SLEEP_CYCLE * 2 / 5);
//	else
//		usleep(SLEEP_CYCLE * (phil->id % 2));

	// TODO while loop here in case we oversleep.
	if (phil->no_of_phils % 2)
//		sync_time = (get_time_in_us() - (phil->inception * 1000) + ((phil->ttd - phil->tte - phil->tts) / 10000 * phil->eat_count)) % (3 * phil->tte);
		sync_time = (get_time_in_us() - (phil->inception * 1000) - phil->no_of_phils * phil->eat_count * 100) % (3 * phil->tte);
	else
//		sync_time = (get_time_in_us() - (phil->inception * 1000) + ((phil->ttd - phil->tte - phil->tts) / 10000 * phil->eat_count)) % (2 * phil->tte);
		sync_time = (get_time_in_us() - (phil->inception * 1000) - phil->no_of_phils * phil->eat_count * 100) % (2 * phil->tte);
	int	current_meal_time_slot = sync_time / phil->tte;
	int time_to_meal;
	fprintf(stderr, "%ld %d sync_time %d time to live %ld\n", get_time_in_ms() - phil->inception, phil->id, sync_time, phil->prev_meal + phil->ttd - get_time_in_us());
	if (phil->no_of_phils % 2 && phil->id == 1)		// 3. slot, 1 phil when odd phils
	{
		if (current_meal_time_slot == 2)
			return ;
		time_to_meal = 2 * phil->tte - sync_time;
	}
	else if (phil->id % 2)	// 1. slot, odd phil
	{
		if (current_meal_time_slot == 0)
			return ;
		time_to_meal = (2 + (phil->no_of_phils % 2)) * phil->tte - sync_time;
	}
	else					// 2. slot, even phil
	{
		if (current_meal_time_slot == 1)
			return ;
		time_to_meal = (sync_time < phil->tte) * (phil->tte - sync_time) + (sync_time >= 2 * phil->tte) * (phil->tte + 3 * phil->tte - sync_time);
	}
	fprintf(stderr, "%ld %d time to meal %d\n", get_time_in_ms() - phil->inception, phil->id, time_to_meal);
	usleep(time_to_meal);


/*		
	sync_time = phil->inception * 1000 + phil->tts +
		phil->eat_count * (phil->tts + phil->tte + phil->tte);
			//(phil->tte * (phil->no_of_phils % 2))) ;//+ phil->no_of_phils * 50 * (phil->eat_count + 1);
	if (phil->no_of_phils % 2 && phil->id == phil->no_of_phils)
		sync_time += (phil->tte) * 2;
	else
		sync_time += (phil->tte) * (phil->id % 2);
	sync_time = sync_time - get_time_in_us();
//	if (phil->prev_meal - phil->inception * 1000 < 60000)
//		sync_time -= 200000;
	fprintf(stderr, "%d sync_time %d\n", phil->id, sync_time);
	if (sync_time < 0)
		return ;
	usleep(sync_time); // TODO death watch sleeping*/
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
			while (!should_die(phil)
				&& (get_time_in_us() - phil->tte) < phil->prev_meal)
				usleep(SLEEP_CYCLE);
//			{
//				if (phil->id == 1)
//					usleep(SLEEP_CYCLE * 2 / 5);
//				else
//					usleep(SLEEP_CYCLE * (phil->id % 2));
//			}
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
		usleep(FORK_SLEEP_CYCLE);
//		if (phil->id == 1)
//			usleep(SLEEP_CYCLE * 2 / 5);
//		else
//			usleep(SLEEP_CYCLE * (phil->id % 2));	
	}
	return (0);
}

void	drop_fork(t_fork *f)
{
	pthread_mutex_unlock(&f->fork_mutex);
	pthread_mutex_lock(&(f->grab_mutex));
	f->taken = 0;
	pthread_mutex_unlock(&(f->grab_mutex));
}
