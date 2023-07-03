/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions_bonus.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 19:56:11 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/03 19:20:00 by tsankola         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <sys/time.h>
#include <stdio.h>
#include <semaphore.h>
#include "philo_bonus.h"

void	phrint(int print_case, t_philosopher *phil)
{
	long int	time;
	sem_t		*phrinty;

	phrinty = sem_open(PRINT_SEMAPHORE_NAME, 0);
	sem_wait(phrinty);
	if (print_case == EAT)
		printf("%ld %d is eating\n", time, phil->id);
	else if (print_case == SLEEP)
		printf("%ld %d is sleeping\n", time, phil->id);
	else if (print_case == FORK_TAKE)
		printf("%ld %d has taken a fork\n", time, phil->id);
	else if (print_case == DIE)
	{
		printf("%ld %d died\n", time, phil->id);
		printf("%d Distance from death %ld\n", phil->id, get_time_in_us() - phil->prev_meal - phil->ttd);
	}
	else if (print_case == THINK)
		printf("%ld %d is thinking\n", time, phil->id);
	sem_post(phrinty);
}

int	should_die(t_philosopher *phil)
{
	sem_t		*morty;

	morty = sem_open(DEATH_SEMAPHORE_NAME, 0);
	if (*phil->death)
	{
		sem_post(morty);
		phil->dead = 1;
		return (-1);
	}
	if (get_time_in_us() - (phil->prev_meal + phil->ttd) > 0)
	{
		*phil->death = 1;
		sem_post(morty);
		phil->dead = 1;
		phrint(DIE, phil);
		return (1);
	}
	sem_post(morty);
	return (0);
}

void	think(t_philosopher *phil)
{
	phrint(THINK, phil);
}

void	deep_think(t_philosopher *phil)
{
	long int	sleep_start;

	if (! phil->dead)
	{
		sleep_start = get_time_in_us();
		phrint(SLEEP, phil);
		while ((get_time_in_us() - phil->tts) < sleep_start)
		{
			usleep(SLEEP_CYCLE);
			if (should_die(phil))
				return ;
		}
	}

}

int	wait_on_mutex(sem_t *semaphore, int *value)
{
	int	ret_val;

	sem_wait(semaphore);
	ret_val = *value;
	sem_post(semaphore);
	if (! ret_val)
		usleep(SLEEP_CYCLE);
	return (ret_val);
}

int	take_fork(t_philosopher *phil)	// I'm thinking fork is necessary here...
{
	sem_t		*grabby;
	sem_t		*forky;
	int			i;

	i = -1;
	grabby = sem_open(GRAB_SEMAPHORE_NAME, 0);
	forky = sem_open(FORK_SEMAPHORE_NAME, 0);
	sem_wait(grabby);
	while (! phil->dead && ++i < 2)
	{
		if (should_die(phil))
		{
			sem_post(grabby);
			sem_close(grabby);
			return (0);
		}
		while (i < 2)
		{
			sem_wait(forky);
			phrint(FORK_TAKE, phil);
		}
		sem_post(grabby);
		usleep(SLEEP_CYCLE);
	}
	sem_close(grabby);
	return (0);
}

void	drop_fork(t_philosopher *phil)
{
	sem_t	*forky;

	forky = sem_open(FORK_SEMAPHORE_NAME, 0);
	sem_post(forky);
	sem_post(forky);
	sem_close(forky);
}

void	eat(t_philosopher *phil)
{
	pthread_t	hands;
	int			success;

	pthread_create(&hands, NULL, (void * (*)(void *))take_fork,
			(void *)phil);
	// Add another value mutex pair for forks.
	pthread_join(hands, &success);

	// How to kill the thread? Should phil->dead be used so that the thread can
	// check that while getting the grab mutex? Another mutex should be
	// implemented for that.
}
