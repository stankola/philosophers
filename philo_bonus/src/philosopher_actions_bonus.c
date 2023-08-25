/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:17:38 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/25 17:08:35 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

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
	phrint(EAT, phil);
	phleep(phil, phil->tte);
	return ;
}

int	take_fork(t_philosopher *phil, t_fork *f)
{
	phrint(FORK_TAKE, phil);
	phrint(FORK_TAKE, phil);
	return (0);
}

inline void	drop_fork(t_fork *f)
{
	return ;
}
