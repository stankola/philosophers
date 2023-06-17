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
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	phil->prev_meal = tv.tv_usec / 1000;
	while (1 && phil->dead != 1)
	{
		deep_think(phil);
		think(phil);
		eat(phil);
		gettimeofday(&tv, NULL);
		if (phil->mm != 0)
			if (--phil->mm == 0)
				break ;
	}
	return (phil);
}

void	*philosophize_testing(t_philosopher *phil)
{
	int	i;

	i = 0;
	while (++i < 100)
		think(phil);
	return (NULL);
}

int	phallocate(t_fork **utensils, t_philosopher **phils, unsigned int args[])
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
	return (0);
}

t_philosopher	*phinitialize(unsigned int a[])
{
	t_fork			*u;
	t_philosopher	*phils;
	unsigned int	i;

	if (phallocate(&u, &phils, a))
		return (NULL);
	i = 0;
	while (++i < a[no_of_phils])
	{
		u[i].taken = 0;
		pthread_mutex_init(&u[i].mutex, NULL);	// Might fail
		phils[i] = (t_philosopher){i + 1, a[time_to_die], a[time_to_eat],
			a[time_to_sleep], a[max_meals], 0, 0, 0, &u[i], &u[i - 1]};
	}
	u[0].taken = 0;
	pthread_mutex_init(&u[0].mutex, NULL);	// Might fail
	phils[0] = (t_philosopher){1, a[time_to_die], a[time_to_eat],
		a[time_to_sleep], a[max_meals], 0, 0, 0, &u[0], &u[a[no_of_phils] - 1]};
	return (phils);
}

void	phree(t_philosopher *phil)
{
	if (phil->r_utensil != NULL)
		free(phil->r_utensil);
	phil->l_utensil = NULL;
	phil->r_utensil = NULL;
	free(phil);
}

pthread_t	*great_commission(t_philosopher *phils, int philc)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(pthread_t) * philc);
	if (threads == NULL)
		return (NULL);
	i = -1;
	while (++i < philc)
		pthread_create(&threads[i], NULL, (void * (*)(void *))philosophize, (void *)&phils[i]);
	return (threads);
}
/* Arguments:
philno, ttd, tte, tts, max_meals
*/
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
	threads = great_commission(phils, args[no_of_phils]);

	i = 0;
	while (i < args[no_of_phils])
		pthread_join(threads[i++], NULL);
	phree(phils);
	return (0);
}
