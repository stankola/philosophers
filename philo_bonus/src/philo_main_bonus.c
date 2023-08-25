/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:33 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/25 16:38:20 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

// Pointers might point to unallocated memory at the end but the caller should
// know not to use them from the return value
static int	phallocate(void)
{
	return (0);
}

// pthread_mutex_destroy might fail, should make sure that threads have
// stopped executing.
static void	phree(void)
{
	return ;
}

void	phinitialize(void)
{
	return ;
}

void	phinish(void)
{
	return ;
}

int	main(int argc, char *argv[])
{
	unsigned int		args[5];
	int volatile		death;
	t_printer_thread	*pt;

	if (parse_args(argc, argv, args) || args[no_of_phils] == 0
		|| (argc == 6 && args[max_meals] == 0))
		return (22);
	if (argc == 5)
		args[max_meals] = 0;
	pt = NULL;
	death = 0;
	return (0);
}
