/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/27 21:10:02 by tsankola         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <fcntl.h>	// For O_CREAT
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "philo_bonus.h"

// sleep
// think
// eat
t_philosopher	*philosophize(t_philosopher *phil)
{
	void		(*actions[3]) (t_philosopher *) = {deep_think, think, eat};
	int			i;
	static volatile int	death = 0;

	phil->prev_meal = get_time_in_us();
	i = phil->id % 2;
	while (! phil->dead)
	{
		actions[i](phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			return (phil);
		should_die(phil);
		i = (i + 1) % 3;
	}
	return (phil);
}

int	phallocate(t_philosopher **phils, unsigned int args[])
{
	*phils = malloc(sizeof(t_philosopher) * args[no_of_phils]);
	if (*phils == NULL)
		return (1);
	return (0);
}

t_philosopher	*phinitialize(unsigned int a[])
{
	t_philosopher	*ps;
	long int		now;
	unsigned int	i;

	if (phallocate(&ps, a))
		return (NULL);
	now = get_time_in_ms();
	i = 0;
	while (++i < a[no_of_phils])
	{
		ps[i] = (t_philosopher){i + 1, a[time_to_die], a[time_to_eat],
			a[time_to_sleep], a[max_meals], 0, 0, now, NULL};
	}
	ps[0] = (t_philosopher){1, a[time_to_die], a[time_to_eat], a[time_to_sleep],
		a[max_meals], 0, 0, now, NULL};
	sem_open(FORK_SEMAPHORE_NAME, 0);
	sem_open(PRINT_SEMAPHORE_NAME, 0);
	return (ps);
}

void	phree(t_philosopher *phil, int philc)
{
	free(phil);
	sem_close(sem_open(FORK_SEMAPHORE_NAME, 0));
	sem_close(sem_open(PRINT_SEMAPHORE_NAME, 0));
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
		pthread_create(&threads[i], NULL, (void * (*)(void *))philosophize,
			(void *)&phils[i]);
	}
	return (threads);
}

// Arguments: philno, ttd, tte, tts, max_meals
int	main(int argc, char *argv[])
{
	unsigned int	args[5];
	t_philosopher	*phils;
	pthread_t		*threads;
	unsigned int	i;

	if (parse_args(argc, argv, args))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	args[time_to_die] *= 1000;
	args[time_to_eat] *= 1000;
	args[time_to_sleep] *= 1000;
	phils = phinitialize(args);
	threads = phacilitate(phils, args[no_of_phils]);
	if (threads != NULL)
	{
		i = 0;
		while (i < args[no_of_phils])
			pthread_join(threads[i++], NULL);
	}
	phree(phils, args[no_of_phils]);
	return (0);
}
