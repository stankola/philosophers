/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:36:39 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
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

t_philosopher	*phinitialize(unsigned int a[], int volatile *death,
	t_printer_thread **pt)
{
	t_fork			*fs;
	t_philosopher	*ps;
	pthread_mutex_t	*mutexes;
	long int		now;
	unsigned int	i;

	if (phallocate(&fs, &ps, a, &mutexes))
		return (NULL);
	printer_thread_init(pt, a[no_of_phils], &mutexes[PRINT_MUTEX_I]);
	now = get_time_in_ms();
	i = 0;
	while (++i <= a[no_of_phils])
	{
		ps[i - 1] = (t_philosopher){i, a[ttd], a[tte], a[tts], a[max_meals],
			0, 0, 0, a[no_of_phils], now, &fs[i % a[no_of_phils]], &fs[i - 1],
			death, mutexes, (volatile t_print_buffer **)&(*pt)->buffer};
		fs[i - 1].taken = 0;
		if (pthread_mutex_init(&fs[i - 1].fork_mutex, NULL))
		{
			phree(&ps, a[no_of_phils]);
			return (NULL);
		}
	}
	return (ps);
}

void	phinish(t_philosopher *phils, pthread_t *threads, unsigned int args[],
	t_printer_thread *pt)
{
	unsigned int	i;

	i = 1;
	if (threads != NULL)
	{
		while (i < args[no_of_phils] + 1)
			pthread_join(threads[i++], NULL);
		printer_thread_stop(pt);
		pthread_join(threads[0], NULL);
		printer_thread_del(&pt);
		free(threads);
	}
	phree(&phils, args[no_of_phils]);
}

int	main(int argc, char *argv[])
{
	unsigned int		args[5];
	t_philosopher		*phils;
	pthread_t			*threads;
	int volatile		death;
	t_printer_thread	*pt;

	if (parse_args(argc, argv, args) || args[no_of_phils] == 0
		|| (argc == 6 && args[max_meals] == 0))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	pt = NULL;
	death = 0;
	phils = phinitialize(args, &death, &pt);
	if (phils == NULL)
		return (-1);
	threads = phacilitate(phils, args[no_of_phils], pt);
	phinish(phils, threads, args, pt);
	return (0);
}
