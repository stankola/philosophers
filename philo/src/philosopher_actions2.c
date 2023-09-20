/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:42:12 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:42:26 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "philo.h"

static t_philosopher	*philosophize(t_philosopher *phil)
{
	void	(*actions[3])(t_philosopher *);
	int		i;

	if (phil == NULL)
		return (NULL);
	actions[0] = &deep_think;
	actions[1] = &think;
	actions[2] = &eat;
	i = 1;
	phil->prev_meal = get_time_in_us();
	while (! should_die(phil))
	{
		(*actions[i])(phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			return (phil);
		i = (i + 1) % 3;
	}
	return (phil);
}

// phleep as in philo sleep
void	phleep(t_philosopher *phil, unsigned int duration)
{
	long int	wake_up;
	long int	time;
	suseconds_t	*interval;

	if (duration <= MEDIUM_SLEEP)
	{
		usleep(duration);
		return ;
	}
	interval = (suseconds_t []){LONG_SLEEP, MEDIUM_SLEEP, SNOOZE};
	time = get_time_in_us();
	wake_up = (time + duration);
	if (wake_up > phil->prev_meal + phil->ttd)
		wake_up = phil->prev_meal + phil->ttd;
	while (time < wake_up && !should_die(phil))
	{
		while (*interval != SNOOZE && (wake_up - time < *interval))
			interval++;
		if (wake_up - time < *interval)
			usleep(wake_up - time);
		else
			usleep(*interval);
		time = get_time_in_us();
	}
}

pthread_t	*phacilitate(t_philosopher *phils, int philc, t_printer_thread *pt)
{
	pthread_t	*threads;
	int			i;
	int			err_check;

	threads = malloc(sizeof(pthread_t) * (philc + 1));
	if (threads == NULL)
		return (NULL);
	i = -1;
	while (++i < philc + 1)
	{
		if (i == 0)
			err_check = pthread_create(&threads[i], NULL,
					(void *(*)(void *))printer_thread, (void *)pt);
		else
		{
			err_check = pthread_create(&threads[i], NULL,
					(void *(*)(void *))philosophize, (void *)&phils[i - 1]);
		}
		if (err_check)
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

	time = (get_time_in_ms() - phil->inception);
	pthread_mutex_lock(&phil->mutexes[PRINT_MUTEX_I]);
	print_buffer_write(*phil->print_buffer, time, phil->id, print_case);
	pthread_mutex_unlock(&phil->mutexes[PRINT_MUTEX_I]);
}

int	should_die(t_philosopher *phil)
{
	if (phil->dead)
		return (1);
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
