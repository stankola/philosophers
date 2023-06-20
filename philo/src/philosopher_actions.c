/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 17:05:24 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/16 17:05:25 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include "philo.h"

/*
Any state change of a philosopher must be formatted as follows:
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
*/

void	phrint(int print_case, t_philosopher *phil)
{
	long int	time;

	time = get_time_in_ms() - phil->inception;
	pthread_mutex_lock(&phil->mutexes[PRINT_MUTEX_I]);
	if (print_case == EAT)
		printf("%ld %d is eating\n", time, phil->id);
	if (print_case == SLEEP)
		printf("%ld %d is sleeping\n", time, phil->id);
	if (print_case == FORK_TAKE)
		printf("%ld %d has taken a fork\n", time, phil->id);
	if (print_case == DIE)
		printf("%ld %d died\n", time, phil->id);
	if (print_case == THINK)
		printf("%ld %d is thinking\n", time, phil->id);
	pthread_mutex_unlock(&phil->mutexes[PRINT_MUTEX_I]);
}

int	should_die(t_philosopher *phil)
{
	pthread_mutex_lock(phil->mutexes);
	if (*phil->death)
	{
		pthread_mutex_unlock(phil->mutexes);
		return (-1);
	}
	pthread_mutex_unlock(phil->mutexes);
	if (get_time_in_us() - (phil->prev_meal + phil->ttd) > 0)
	{
		printf("Distance from death %ld\n", get_time_in_us() - phil->prev_meal - phil->ttd);
		return (1);
	}
	return (0);
}

void	die(t_philosopher *phil)
{
	pthread_mutex_lock(phil->mutexes);
	phil->dead = 1;
	*phil->death = 1;
	pthread_mutex_unlock(phil->mutexes);
	phrint(DIE, phil);
}

void	eat(t_philosopher *phil)
{
	if (phil->id % 2)
	{
		take_fork(phil, phil->r_utensil);
		if(! phil->dead)
			take_fork(phil, phil->l_utensil);
	}
	else
	{
		take_fork(phil, phil->l_utensil);
		if(! phil->dead)
			take_fork(phil, phil->r_utensil);
	}
	if (should_die(phil) > 0)
		die(phil);
	if(! phil->dead)
	{
		phil->prev_meal = get_time_in_us();
		phrint(EAT, phil);
		while (!phil->dead && (get_time_in_us() - phil->tte) < phil->prev_meal)
			usleep(SLEEP_CYCLE);
	}
	drop_fork(phil->r_utensil);
	drop_fork(phil->l_utensil);
}

void	think(t_philosopher *phil)
{
	long int	think_start;

	think_start = get_time_in_us();
	phrint(THINK, phil);
}

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	long int	sleep_start;

	sleep_start = get_time_in_us();
	phrint(SLEEP, phil);
	while (!phil->dead && (get_time_in_us() - phil->tts) < sleep_start)
	{
		usleep(SLEEP_CYCLE);
		if (should_die(phil) > 0)
			die(phil);
	}
}

void	take_fork(t_philosopher *phil, t_fork *f)
{
	while (! phil->dead)
	{
		pthread_mutex_lock(&(f->grab_mutex));
		if (f->taken == 0)
		{
			f->taken = 1;
			pthread_mutex_lock(&(f->fork_mutex));
			phrint(FORK_TAKE, phil);
			pthread_mutex_unlock(&(f->grab_mutex));
			return ;
		}
		pthread_mutex_unlock(&(f->grab_mutex));
		usleep(SLEEP_CYCLE);
		if (should_die(phil) > 0)
			die(phil);
	}
}

void	drop_fork(t_fork *f)
{
	pthread_mutex_lock(&(f->grab_mutex));
	f->taken = 0;
	pthread_mutex_unlock(&f->fork_mutex);
	pthread_mutex_unlock(&(f->grab_mutex));
}
