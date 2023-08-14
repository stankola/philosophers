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

#include "philo.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

static t_philosopher	*philosophize(t_philosopher *phil)
{
	void				(*actions[3])(t_philosopher *);
	int					i;

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
int	phleep(t_philosopher *phil, suseconds_t duration)
{
	long int	wake_up;
	suseconds_t	*interval;
	long int	time;

	if (duration <= MEDIUM_SLEEP_INTERVAL)
	{
		usleep(duration);
		return (1);
	}
	interval = (suseconds_t []){LONG_SLEEP_INTERVAL, MEDIUM_SLEEP_INTERVAL, SHORT_SLEEP_INTERVAL};
	time = get_time_in_us();
	wake_up = time + duration;
	if (wake_up > phil->prev_meal + phil->ttd)
		wake_up = phil->prev_meal + phil->ttd;
	fprintf(stderr, "%ld %d sleeping for %ld\n", get_time_in_ms() - phil->inception, phil->id, (wake_up - get_time_in_us()) / 100 * 90);
	if (duration > LONG_SLEEP_INTERVAL)
		usleep((wake_up - time) / 100 * 80);
	if (get_time_in_us() > wake_up)
		fprintf(stderr, "%d overslept by %ld!\n", phil->id, get_time_in_us() - wake_up);
	while (time < wake_up)
	{
		if (should_die(phil))
			return (0);
		while (*interval != SHORT_SLEEP_INTERVAL && (wake_up - time < *interval))
			interval++;
		if (wake_up - time < *interval)
			usleep(wake_up - time);
		else
			usleep(*interval);
		time = get_time_in_us();
	}
	return (1);
}

pthread_t	*phacilitate(t_philosopher *phils, int philc, t_printer_thread *pr_thread)
{
	pthread_t	*threads;
	int			i;
	int			err_check;

	threads = malloc(sizeof(pthread_t) * philc + 1);
	if (threads == NULL)
		return (NULL);
	i = -1;
	while (++i < philc + 1)
	{
		if (i == 0)
			err_check = pthread_create(&threads[i], NULL, (void *(*)(void *))printer_thread, (void *)pr_thread);
		else
		       	err_check = pthread_create(&threads[i], NULL, (void *(*)(void *))philosophize, (void *)&phils[i]);
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
	if (print_case == DIE)
		print_buffer_write(*phil->print_buffer, time, phil->id, print_case);
//		printf("%ld %d died\n", time, phil->id);
	else
	{
		pthread_mutex_lock(&phil->mutexes[DEATH_MUTEX_I]);
		if (*phil->death == 0)
		{
			print_buffer_write(*phil->print_buffer, time, phil->id, print_case);
/*			if (print_case == EAT)
				printf("%ld %d is eating\n", time, phil->id);
			else if (print_case == SLEEP)
				printf("%ld %d is sleeping\n", time, phil->id);
			else if (print_case == FORK_TAKE)
				printf("%ld %d has taken a fork\n", time, phil->id);
			else if (print_case == THINK)
				printf("%ld %d is thinking\n", time, phil->id);
*/		}
		pthread_mutex_unlock(&phil->mutexes[DEATH_MUTEX_I]);
	}
	pthread_mutex_unlock(&phil->mutexes[PRINT_MUTEX_I]);
}

inline int	should_die(t_philosopher *phil)
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
