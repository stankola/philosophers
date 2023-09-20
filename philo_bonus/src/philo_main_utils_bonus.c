/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:00:44 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 22:08:54 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "philo_bonus.h"

static int	phork(t_philosopher *phils, int philc)
{
	int	i;

	i = -1;
	while (++i < philc)
	{
		phils[i].pid = fork();
		if (phils[i].pid == 0)
			philosophize(&phils[i]);
		if (phils[i].pid < 0)
			break ;
	}
	return (i);
}

static void	stop(t_philosopher *phils, int i, pid_t	exclude_pid,
	sem_t *printsem)
{
	int	status;

	while (i >= 0)
	{
		if (exclude_pid != 0 && phils[i].pid != exclude_pid)
			kill(phils[i].pid, SIGINT);
		i--;
	}
	while (waitpid(-1, &status, 0) >= 0)
		;
	if (printsem != NULL)
		sem_close(printsem);
}

void	phacilitate(t_philosopher *phils, int philc)
{
	int		i;
	int		status;
	pid_t	exclude_pid;
	sem_t	*printsem;

	printsem = NULL;
	exclude_pid = 0;
	i = phork(phils, philc);
	if (i == philc)
	{
		while (--i > 0)
		{
			exclude_pid = waitpid(-1, &status, 0);
			if (!WIFSIGNALED(status) && WEXITSTATUS(status) == EXIT_STARVED)
				break ;
			else if (!WIFSIGNALED(status) && WEXITSTATUS(status) != EXIT_FULL)
				printsem = sem_open(PRINT_SEM_NAME, 0);
		}
	}
	stop(phils, i, exclude_pid, printsem);
}
