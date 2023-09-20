/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:27:09 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:43:05 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include "philo.h"

inline long int	get_time_in_us(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000000 + now.tv_usec);
}

inline long int	get_time_in_ms(void)
{
	struct timeval	now;

	gettimeofday(&now, NULL);
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}

static int	time_to_meal_calc(t_philosopher *phil, int time_slot, int length,
	int cycle_time)
{
	int	time_to_meal;

	if (phil->no_of_phils % 2 && phil->id == 1)
	{
		if (time_slot == 2)
			return (0);
		time_to_meal = 2 * length - cycle_time;
	}
	else if (phil->id % 2)
	{
		if (time_slot == 0)
			return (0);
		time_to_meal = (2 + phil->no_of_phils % 2) * length - cycle_time;
	}
	else
	{
		if (time_slot == 1)
			return (0);
		time_to_meal = (cycle_time < length) * (length - cycle_time)
			+ (cycle_time >= 2 * length) * (length + 3 * length - cycle_time);
	}
	return (time_to_meal);
}

int	get_time_to_meal(t_philosopher *phil)
{
	int	simulation_time;
	int	duration;
	int	cycle_time;
	int	time_slot;
	int	time_to_meal;

	simulation_time = get_time_in_us() - (phil->inception * 1000);
	duration = phil->ttd / (2 + phil->no_of_phils % 2)
		- 25 * ((phil->ttd - (2 + phil->no_of_phils % 2) * phil->tte) / 100);
	if (duration != 0)
	{
		cycle_time = simulation_time % ((2 + phil->no_of_phils % 2) * duration);
		time_slot = cycle_time / duration;
	}
	else
	{
		cycle_time = 0;
		time_slot = 1;
	}
	time_to_meal = time_to_meal_calc(phil, time_slot, duration, cycle_time);
	return (time_to_meal);
}
