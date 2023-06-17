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
#include "philo.h"

/*
Any state change of a philosopher must be formatted as follows:
timestamp_in_ms X has taken a fork
timestamp_in_ms X is eating
timestamp_in_ms X is sleeping
timestamp_in_ms X is thinking
timestamp_in_ms X died
*/
void	die(t_philosopher *phil)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	phil->dead = 1;	
	printf("%ld %d died\n", tv.tv_usec / 1000, phil->id);
}

void	eat(t_philosopher *phil)
{
	struct timeval	tv;
	
	take_fork(phil->r_utensil);
	gettimeofday(&tv, NULL);
	printf("%ld %d has taken a fork\n", tv.tv_usec / 1000, phil->id);
	take_fork(phil->l_utensil);
	gettimeofday(&tv, NULL);
	printf("%ld %d has taken a fork\n", tv.tv_usec / 1000, phil->id);
	if (phil->prev_meal + phil->ttd < tv.tv_usec / 1000)
		die(phil);
	else
	{
		printf("%ld %d is eating\n", tv.tv_usec / 1000, phil->id);
		usleep(phil->tte * 1000);
		gettimeofday(&tv, NULL);
		phil->prev_meal = tv.tv_usec / 1000;
	}
	drop_fork(phil->r_utensil);
	drop_fork(phil->l_utensil);
}

void	think(t_philosopher *phil)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld %d is thinking\n", tv.tv_usec / 1000, phil->id);
	usleep(phil->ttd * 100);
	if (phil->prev_meal + phil->ttd < tv.tv_usec / 1000)
		die(phil);
}

// ie. sleep
void	deep_think(t_philosopher *phil)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	printf("%ld %d is sleeping\n", tv.tv_usec / 1000, phil->id);	
	usleep(phil->tts * 1000);
	if (phil->prev_meal + phil->ttd < tv.tv_usec / 1000)
		die(phil);
}

void	take_fork(t_fork *f)
{
	pthread_mutex_lock(&(f->mutex));
}

void	drop_fork(t_fork *f)
{
	pthread_mutex_unlock(&f->mutex);
}
