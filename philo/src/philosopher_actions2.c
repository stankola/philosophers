/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:42:12 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/05 17:42:13 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "philo.h"

static t_philosopher	*philosophize(t_philosopher *phil)
{
	void				(*actions[3])(t_philosopher *);
	int					i;
	static volatile int	death = 0;

	actions[0] = &deep_think;
	actions[1] = &think;
	actions[2] = &eat;
	phil->death = &death;
	i = phil->id % 2;
	phil->prev_meal = get_time_in_us();
	while (! phil->dead)
	{
		(*actions[i])(phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			return (phil);
		i = (i + 1) % 3;
	}
	phil->death = NULL;
	return (phil);
}

pthread_t	*phacilitate(t_philosopher *phils, int philc)
{
	pthread_t		*threads;
	int				i;

	threads = malloc(sizeof(pthread_t) * philc);
	if (threads == NULL)
		return (NULL);
	i = -1;
	while (++i < philc)
	{
		if (pthread_create(&threads[i], NULL, (void *(*)(void *))philosophize,
			(void *)&phils[i]))
		{
			free(threads);
			return (NULL);
		}
	}
	return (threads);
}

void	phrint(int print_case, t_philosopher *phil)
{
	long int	time;

	pthread_mutex_lock(&phil->mutexes[PRINT_MUTEX_I]);
	time = get_time_in_ms() - phil->inception;
	if (print_case == EAT)
		printf("%ld %d is eating\n", time, phil->id);
	else if (print_case == SLEEP)
		printf("%ld %d is sleeping\n", time, phil->id);
	else if (print_case == FORK_TAKE)
		printf("%ld %d has taken a fork\n", time, phil->id);
	else if (print_case == DIE)
		printf("%ld %d died\n", time, phil->id);
	else if (print_case == THINK)
		printf("%ld %d is thinking\n", time, phil->id);
	pthread_mutex_unlock(&phil->mutexes[PRINT_MUTEX_I]);
}

int	should_die(t_philosopher *phil)
{
	pthread_mutex_lock(&phil->mutexes[DEATH_MUTEX_I]);
	if (*phil->death)
	{
		pthread_mutex_unlock(&phil->mutexes[DEATH_MUTEX_I]);
		phil->dead = 1;
		return (-1);
	}
	if (get_time_in_us() - (phil->prev_meal + phil->ttd) > 0)
	{
		*phil->death = 1;
		pthread_mutex_unlock(&phil->mutexes[DEATH_MUTEX_I]);
		phil->dead = 1;
		phrint(DIE, phil);
		return (1);
	}
	pthread_mutex_unlock(&phil->mutexes[DEATH_MUTEX_I]);
	return (0);
}
