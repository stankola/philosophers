/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions2_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:42:12 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 21:48:41 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include "philo_bonus.h"

static void	init_phil(t_philosopher *phil)
{
	printer_thread_init(&phil->stenographer, 1, phil->id);
	phil->utensils = sem_open(FORK_SEM_NAME, 0);
	phil->utensil_pairs = sem_open(FORK2_SEM_NAME, 0);
	phil->hand_sem = sem_open(phil->hands_name, 0);
}

void	philosophize(t_philosopher *phil)
{
	pthread_t	sten;
	void		(**actions)(t_philosopher *);
	int			i;

	actions = (void (*[3])(t_philosopher *)){deep_think, think, eat};
	i = 1;
	init_phil(phil);
	pthread_create(&sten, NULL,
		(void *(*)(void *))printer_thread, phil->stenographer);
	phil->prev_meal = get_time_in_us();
	while (! should_die(phil))
	{
		(*actions[i])(phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			break ;
		i = (i + 1) % 3;
	}
	printer_thread_stop(phil->stenographer);
	pthread_join(sten, NULL);
	printer_thread_del(&phil->stenographer);
	if (phil->dead)
		exit(EXIT_STARVED);
	exit(EXIT_FULL);
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

int	should_die(t_philosopher *phil)
{
	if (phil->dead)
		return (1);
	if (get_time_in_us() - phil->prev_meal > phil->ttd)
	{
		phil->dead = 1;
		phrint(phil, DIE);
		return (1);
	}
	return (0);
}
