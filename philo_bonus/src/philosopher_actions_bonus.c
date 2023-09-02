/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:17:38 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/02 22:05:11 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	phrint(phil->stenographer, get_time_in_ms(), phil->id, SLEEP);
	phleep(phil, phil->tts);
}

void	think(t_philosopher *phil)
{
	int	time_to_meal;

	phrint(phil->stenographer, get_time_in_ms(), phil->id, THINK);
	return ;
}

void	eat(t_philosopher *phil)
{
	sem_wait(phil->utensil_pairs);
	if (should_die(phil))
	{
		sem_post(phil->utensil_pairs);
		return ;
	}
	take_forks(phil);
	phrint(phil->stenographer, get_time_in_ms(), phil->id, EAT);
	phleep(phil, phil->tte);
	drop_forks(phil);
	sem_post(phil->utensil_pairs);
	return ;
}

int	take_forks(t_philosopher *phil)
{
	sem_wait(phil->utensils);
	if (!should_die(phil))
	{
		phrint(phil->stenographer, get_time_in_ms(), phil->id, FORK_TAKE);
		sem_wait(phil->utensils);
		if (!should_die(phil))
		{
			phrint(phil->stenographer, get_time_in_ms(), phil->id, FORK_TAKE);
			return (0);
		}
		sem_post(phil->utensils);
	}
	sem_post(phil->utensils);
	return (1);
}

inline void	drop_forks(t_philosopher *phil)
{
	sem_post(phil->utensils);
	sem_post(phil->utensils);
	return ;
}
