/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:00:44 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/21 10:15:05 by tsankola         ###   ########.fr       */
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

static void	stop(t_philosopher *phils, int i)
{
	int	status;

	if (i > 0)
		i = phils[0].no_of_phils - 1;
	while (i >= 0)
		kill(phils[i--].pid, SIGINT);
	while (waitpid(-1, &status, 0) >= 0)
		;
}

void	phacilitate(t_philosopher *phils, int philc)
{
	int		i;
	int		status;

	i = phork(phils, philc);
	if (i == philc)
	{
		while (--i >= 0)
		{
			waitpid(-1, &status, 0);
			if (!WIFSIGNALED(status) && WEXITSTATUS(status) == EXIT_STARVED)		// Phil starved, no signal. Go kill everyone.
				break ;
			// Either a Phil ate all his meals or ^C was pressed. Wait for others to die.
		}
	}
	stop(phils, i);
}

void	init_philosopher(t_philosopher *p, unsigned int a[5], int id, long time)
{
	if (!p)
		return ;
	p->pid = -1;
	p->id = id;
	p->ttd = a[ttd];
	p->tte = a[tte];
	p->tts = a[tts];
	p->mm = a[max_meals];
	p->eat_count = 0;
	p->dead = 0;
	p->prev_meal = 0;
	p->no_of_phils = a[no_of_phils];
	p->inception = time;
	p->holding_forks = 0;
	name_generator(p->hands_name, HANDS_SEM_NAME, p->id);
	p->utensil_pairs = NULL;
	p->utensils = NULL;
	p->hand_sem = NULL;
}
