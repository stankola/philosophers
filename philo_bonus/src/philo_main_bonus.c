/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/27 16:06:35 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

 void	phree(void)
{
	return ;
}

void	phinitialize(unsigned int a[5], t_philosopher **p)
{
	int		i;
	long	now;
	
	now = get_time_in_ms();
	*p = malloc(sizeof(t_philosopher) * a[no_of_phils]);
	if (*p == NULL)
		return ;
	sem_open(PRINT_SEM_NAME, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 1);
	i = -1;
	while (++i < a[no_of_phils])
		(*p)[i] = (t_philosopher){i + 1, a[ttd], a[tte], a[tts], a[max_meals],
				0, 0, 0, a[no_of_phils], now, NULL, NULL};
	return ;
}

void	phinish(pid_t *children, t_philosopher *phils)
{
	// Unlink semaphores.
	// Kill processes
	return ;
}

#include <stdio.h>
int	main(int argc, char *argv[])
{
	unsigned int	args[5];
	sem_t			*death;
	pid_t			*children;
	t_philosopher	*phils;

 	if (parse_args(argc, argv, args) || args[no_of_phils] == 0
		|| (argc == 6 && args[max_meals] == 0))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	phallocate(args, &phils);
	return (0);
}
