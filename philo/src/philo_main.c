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

/*
void	philosophize(t_philosopher phil)
{
	struct timeval	prev_meal;
	int				etd;

	gettimeofday(&prev_meal, NULL);		// might fail
	while (1)
	{
		think(phil);
		deep_think(phil);
		eat(phil);
	}
}
*/
t_philosopher	*phinitialize(unsigned int a[]) // TODO: Consider a without max_meals
{
	t_fork			*u;
	t_philosopher	*phils;
	unsigned int	i;

	phils = malloc(sizeof(t_philosopher) * a[no_of_phils]);
	if (phils == NULL)
		return (NULL);
	u = malloc(sizeof(t_fork) * a[no_of_phils]);
	if (u == NULL)
	{
		free(phils);
		return (NULL);
	}
	i = 0;
	while (++i < a[no_of_phils])
	{
		u[i].taken = 0;
		phils[i] = (t_philosopher){i + 1, a[time_to_die], a[time_to_eat],
			a[time_to_sleep], a[max_meals], 0, 0, &u[i], &u[i - 1]};
	}
	u[0].taken = 0;
	phils[0] = (t_philosopher){1, a[time_to_die], a[time_to_eat],
		a[time_to_sleep], a[max_meals], 0, 0, &u[0], &u[a[no_of_phils - 1]]};
	return (phils);
}

void	phree(t_philosopher *phil)
{
	if (phil->r_utensil != NULL)
		free(phil->r_utensil);
	if (phil->l_utensil != NULL)
		free(phil->l_utensil);
	phil->l_utensil = NULL;
	phil->r_utensil = NULL;
	free(phil);
}

/* Arguments:
number_of_philosophers
time_to_die
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
Let valid values belong to the range [0,UINT_MAX]
*/

int	main(int argc, char *argv[])
{
	unsigned int			args[5];
	t_philosopher			*phils;

	if (parse_args(argc, argv, args))
		return (22);
	if (argc == 5)
		args[max_meals] = -1;
	phils = phinitialize(args);
//	for (unsigned int i = 0; i < args[no_of_phils]; i++)
//		printf("%d %d %d %d %d\n", phils[i].id, phils[i].ttd, phils[i].mm, phils[i].r_utensil->taken, phils[i].l_utensil->taken);
	return (0);
}