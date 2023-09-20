/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 22:05:57 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "philo_bonus.h"

static void	init_semaphores(t_philosopher *phils, int philc)
{
	sem_t	*sems[3];

	sem_unlink(PRINT_SEM_NAME);
	sem_unlink(FORK_SEM_NAME);
	sem_unlink(FORK2_SEM_NAME);
	sems[0] = sem_open(PRINT_SEM_NAME, O_RDWR | O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR, 1);
	sems[1] = sem_open(FORK_SEM_NAME, O_RDWR | O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR, philc);
	sems[2] = sem_open(FORK2_SEM_NAME, O_RDWR | O_CREAT | O_EXCL,
			S_IRUSR | S_IWUSR, philc / 2);
	sem_close(sems[0]);
	sem_close(sems[1]);
	sem_close(sems[2]);
	while (--philc >= 0)
	{
		phils[philc].hands_name = name_generator(HANDS_SEM_NAME, philc + 1);
		sem_unlink(phils[philc].hands_name);
		phils[philc].hand_sem = sem_open(phils[philc].hands_name,
				O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
		sem_close(phils[philc].hand_sem);
	}
}

static void	phinitialize(unsigned int a[5], t_philosopher **p)
{
	int		i;
	long	now;

	now = get_time_in_ms();
	*p = malloc(sizeof(t_philosopher) * a[no_of_phils]);
	if (*p == NULL)
		return ;
	i = -1;
	while (++i < (int)a[no_of_phils])
		(*p)[i] = (t_philosopher){-1, i + 1, a[ttd], a[tte], a[tts],
			a[max_meals], 0, 0, 0, a[no_of_phils], now, 0,
			NULL, NULL, NULL, NULL, NULL};
	init_semaphores(*p, a[no_of_phils]);
}

static void	phinish(t_philosopher *phils)
{
	sem_unlink(PRINT_SEM_NAME);
	sem_unlink(FORK_SEM_NAME);
	sem_unlink(FORK2_SEM_NAME);
	free(phils);
	return ;
}

int	main(int argc, char *argv[])
{
	unsigned int	args[5];
	t_philosopher	*phils;

	if (parse_args(argc, argv, args) || args[no_of_phils] == 0
		|| (argc == 6 && args[max_meals] == 0))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	phinitialize(args, &phils);
	phacilitate(phils, args[no_of_phils]);
	phinish(phils);
	return (0);
}
