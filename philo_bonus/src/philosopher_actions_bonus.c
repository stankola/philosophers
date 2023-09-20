/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:17:38 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 21:55:50 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/time.h>
#include "philo_bonus.h"

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	phrint(phil, SLEEP);
	phleep(phil, phil->tts);
}

void	think(t_philosopher *phil)
{
	phleep(phil, get_time_to_meal(phil));
	phrint(phil, THINK);
	return ;
}

void	eat(t_philosopher *phil)
{
	if (!take_forks(phil))
		return ;
	if (!should_die(phil))
	{
		phil->prev_meal = get_time_in_us();
		phrint(phil, EAT);
		phleep(phil, phil->tte);
	}
	drop_forks(phil);
	return ;
}

static void	*get_fork_pair(t_philosopher *phil)
{
	sem_wait(phil->utensil_pairs);
	sem_wait(phil->hand_sem);
	phil->holding_forks = 1;
	sem_post(phil->hand_sem);
	return (NULL);
}

// Optimization: No need to check for dying between getting the forks because
// they should be available if we have utensil_pairs. Congestion might cause
// a delay but it shouldn't be significant.
// Returns 1 on success, 0 on failure
int	take_forks(t_philosopher *phil)
{
	pthread_t		pairtaker;

	pthread_create(&pairtaker, NULL, (void*(*)(void *))get_fork_pair, phil);
	while (!should_die(phil))
	{
		sem_wait(phil->hand_sem);
		if (phil->holding_forks == 1)
		{
			pthread_join(pairtaker, NULL);
			sem_post(phil->hand_sem);
			break ;
		}
		sem_post(phil->hand_sem);
		phleep(phil, SNOOZE);
	}
	sem_wait(phil->utensils);
	sem_wait(phil->utensils);
	return (1);
}

void	drop_forks(t_philosopher *phil)
{
	sem_post(phil->utensils);
	sem_post(phil->utensils);
	phil->holding_forks = 0;
	sem_post(phil->utensil_pairs);
	return ;
}
