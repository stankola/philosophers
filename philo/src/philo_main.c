/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/05 00:57:57 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include "philo.h"

// Pointers might point to unallocated memory at the end but the caller should
// know not to use them from the return value
static int	phallocate(t_fork **utensils, t_philosopher **phils,
	unsigned int args[], pthread_mutex_t **mutexes)
{
	*phils = malloc(sizeof(t_philosopher) * args[no_of_phils]);
	if (*phils == NULL)
		return (-1);
	*utensils = malloc(sizeof(t_fork) * args[no_of_phils]);
	if (*utensils == NULL)
	{
		free(*phils);
		return (-1);
	}
	*mutexes = malloc(sizeof(pthread_mutex_t) * 2);
	if (*mutexes == NULL)
	{
		free(*utensils);
		free(*phils);
		return (-1);
	}
	if (pthread_mutex_init(*mutexes, NULL)
		|| pthread_mutex_init(&((*mutexes)[PRINT_MUTEX_I]), NULL))
	{
		free(*utensils);
		free(*phils);
		free(*mutexes);
		return (-1);
	}
	return (0);
}

// pthread_mutex_destroy might fail, should make sure that threads have
// stopped executing.
static void	phree(t_philosopher **phils, int philc)
{
	int	i;

	i = 0;
	while (i < philc)
	{
		pthread_mutex_destroy(&(*phils + i)->r_utensil->fork_mutex);
		pthread_mutex_destroy(&(*phils + i)->r_utensil->grab_mutex);
		i++;
	}
	pthread_mutex_destroy(&(*phils)->mutexes[DEATH_MUTEX_I]);
	pthread_mutex_destroy(&(*phils)->mutexes[PRINT_MUTEX_I]);
	if ((*phils)->l_utensil != NULL)
		free((*phils)->l_utensil);
	free((*phils)->mutexes);
	free((*phils));
	*phils = NULL;
}

static int	finitialize(t_fork *f)
{
	f->taken = 0;
	if (pthread_mutex_init(&f->fork_mutex, NULL)
		|| pthread_mutex_init(&f->grab_mutex, NULL))
		return (-1);
	return (0);
}

t_philosopher	*phinitialize(unsigned int a[])
{
	t_fork			*fs;
	t_philosopher	*ps;
	pthread_mutex_t	*mutexes;
	long int		now;
	unsigned int	i;

	if (phallocate(&fs, &ps, a, &mutexes))
		return (NULL);
	now = get_time_in_ms();
	i = 0;
	while (++i <= a[no_of_phils])
	{
		ps[i - 1] = (t_philosopher){i, a[ttd], a[tte], a[tts], a[max_meals],
			0, 0, 0, a[no_of_phils], now, &fs[i % a[no_of_phils]], &fs[i - 1], NULL, mutexes};
		if (finitialize(&fs[i - 1]))
		{
			phree(&ps, a[no_of_phils]);
			return (NULL);
		}
	}
	return (ps);
}

int	main(int argc, char *argv[])
{
	unsigned int	args[5];
	t_philosopher	*phils;
	pthread_t		*threads;
	unsigned int	i;

	if (parse_args(argc, argv, args) || args[no_of_phils] == 0
		|| (argc == 6 && args[max_meals] == 0))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	phils = phinitialize(args);
	if (phils == NULL)
		return (-1);
	threads = phacilitate(phils, args[no_of_phils]);
	if (threads == NULL)
	{
		phree(&phils, args[no_of_phils]);
		return (-1);
	}
	i = 0;
	while (i < args[no_of_phils])
		pthread_join(threads[i++], NULL);
	free(threads);
	phree(&phils, args[no_of_phils]);
	return (0);
}
