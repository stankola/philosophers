/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/16 14:26:34 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include "philo.h"

// sleep
// think
// eat
t_philosopher	*philosophize(t_philosopher *phil)
{
	void		(*actions[3]) (t_philosopher *) = {deep_think, think, eat};
	int			i;
	static int	death = 0;

	phil->prev_meal = get_time_in_us();
	i = phil->id % 2;
	while (! phil->dead)
	{
		actions[i](phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			return (phil);
		pthread_mutex_lock(phil->death_mutex);
		if (death)
		{
			pthread_mutex_unlock(phil->death_mutex);
			return (phil);
		}
		pthread_mutex_unlock(phil->death_mutex);
		i = (i + 1) % 3;
	}
	pthread_mutex_lock(phil->death_mutex);
	death = 1;
	pthread_mutex_unlock(phil->death_mutex);
	return (phil);
}

int	phallocate(t_fork **utensils, t_philosopher **phils, unsigned int args[],
	pthread_mutex_t **death_mutex)
{
	*phils = malloc(sizeof(t_philosopher) * args[no_of_phils]);
	if (*phils == NULL)
		return (1);
	*utensils = malloc(sizeof(t_fork) * args[no_of_phils]);
	if (*utensils == NULL)
	{
		free(*phils);
		return (1);
	}
	*death_mutex = malloc(sizeof(pthread_mutex_t));
	if (*death_mutex == NULL)
	{
		free(*utensils);
		free(*phils);
		return (1);
	}
	pthread_mutex_init(*death_mutex, NULL);	// could fail
	return (0);
}

int	finitialize(t_fork *f)
{
	f->taken = 0;
	if (pthread_mutex_init(&f->fork_mutex, NULL) != 0
		|| pthread_mutex_init(&f->taken_mutex, NULL) != 0)
		return (0);
	return (1);
}

t_philosopher	*phinitialize(unsigned int a[])
{
	// What if tts + tte > ttd ??
	t_fork			*fs;
	t_philosopher	*ps;
	pthread_mutex_t	*death_mutex;
	long int		now;
	unsigned int	i;

	if (phallocate(&fs, &ps, a, &death_mutex))
		return (NULL);
	now = get_time_in_ms();
	i = 0;
	while (++i < a[no_of_phils])
	{
		finitialize(&fs[i]); // might fail
		ps[i] = (t_philosopher){i + 1, a[time_to_die], a[time_to_eat],
			a[time_to_sleep], a[max_meals], 0, 0, 0, now, &fs[i], &fs[i - 1], death_mutex};
	}
	finitialize(&fs[0]); // might fail
	ps[0] = (t_philosopher){1, a[time_to_die], a[time_to_eat], a[time_to_sleep],
		a[max_meals], 0, 0, 0, now, &fs[0], &fs[a[no_of_phils] - 1], death_mutex};
	return (ps);
}

void	phree(t_philosopher *phil)
{
		// TODO: close mutexes
	if (phil->r_utensil != NULL)
		free(phil->r_utensil);
	phil->l_utensil = NULL;
	phil->r_utensil = NULL;
	free(phil);
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
	phils = phinitialize(args);
	threads = phacilitate(phils, args[no_of_phils]);
	if (threads != NULL)
	{
		i = 0;
		while (i < args[no_of_phils])
			pthread_join(threads[i++], NULL);
	}
	phree(phils);
	return (0);
}
