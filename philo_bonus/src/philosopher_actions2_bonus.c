/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_actions2_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/05 17:42:12 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:34:16 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include "philo_bonus.h"

static void	philosophize(t_philosopher *phil)
{
	pthread_t	sten;
	void		(**actions)(t_philosopher *);
	int			i;

	actions = (void (*[3])(t_philosopher *)){deep_think, think, eat};
	i = 1;
	printer_thread_init(&phil->stenographer, 1, phil->id);
	phil->utensils = sem_open(FORK_SEM_NAME, 0);
	phil->utensil_pairs = sem_open(FORK2_SEM_NAME, 0);
	pthread_create(&sten, NULL,
		(void *(*)(void *))printer_thread, phil->stenographer);
	phil->prev_meal = get_time_in_us();
	while (! should_die(phil))
	{
		(*actions[i])(phil);
		if ((i + 1) % 3 == 0 && ! phil->dead && phil->mm != 0 && ! --phil->mm)
			break ;
		i = (i + 1) % 3;
	}
	printer_thread_stop(phil->stenographer);
	pthread_join(sten, NULL);
	printer_thread_del(&phil->stenographer);
	if (phil->dead)
		exit(EXIT_STARVED);
	exit(EXIT_SATIATED);
}

// phleep as in philo sleep
void	phleep(t_philosopher *phil, unsigned int duration)
{
	long int	wake_up;
	long int	time;
	suseconds_t	*interval;

	if (duration <= MEDIUM_SLEEP)
	{
		usleep(duration);
		return ;
	}
	interval = (suseconds_t []){LONG_SLEEP, MEDIUM_SLEEP, SNOOZE};
	time = get_time_in_us();
	wake_up = (time + duration);
	if (wake_up > phil->prev_meal + phil->ttd)
		wake_up = phil->prev_meal + phil->ttd;
	while (time < wake_up && !should_die(phil))
	{
		while (*interval != SNOOZE && (wake_up - time < *interval))
			interval++;
		if (wake_up - time < *interval)
			usleep(wake_up - time);
		else
			usleep(*interval);
		time = get_time_in_us();
	}
}

/*
pthread_t seems to leak by default when killed by signal
==3568== 272 bytes in 1 blocks are possibly lost in loss record 16 of 20
==3568==    at 0x48455EF: calloc (vg_replace_malloc.c:1328)
==3568==    by 0x4010D52: calloc (rtld-malloc.h:44)
==3568==    by 0x4010D52: allocate_dtv (dl-tls.c:375)
==3568==    by 0x401174D: _dl_allocate_tls (dl-tls.c:634)
==3568==    by 0x48E2BFE: allocate_stack (allocatestack.c:423)
==3568==    by 0x48E2BFE: pthread_create@@GLIBC_2.34 (pthread_create.c:650)
==3568==    by 0x10A712: philosophize (in /home/tsankola/hive/...)
==3568==    by 0x10AAAC: phacilitate (in /home/tsankola/hive/...)
==3568==    by 0x10971B: main (in /home/tsankola/hive/...)
 */
static void	stop(t_philosopher *phils, int i, pid_t	exclude_pid)
{
	int		status;
	sem_t	*printsem;

	printsem = sem_open(PRINT_SEM_NAME, 0);
	while (--i >= 0)
	{
		if (exclude_pid != 0 && phils[i].pid != exclude_pid)
			kill(phils[i].pid, SIGINT);
	}
	while (waitpid(-1, &status, 0) >= 0)
		;
	sem_close(printsem);
}

void	phacilitate(t_philosopher *phils, int philc)
{
	int		i;
	int		status;
	pid_t	exclude_pid;

	i = -1;
	exclude_pid = 0;
	while (++i < philc)
	{
		phils[i].pid = fork();
		if (phils[i].pid == 0)
			philosophize(&phils[i]);
		if (phils[i].pid < 0)
			break ;
	}
	if (i == philc)
	{
		while (--i > 0)
		{
			exclude_pid = waitpid(-1, &status, 0);
			if (WEXITSTATUS(status) == EXIT_STARVED)
				break ;
		}
	}
	stop(phils, i, exclude_pid);
}

int	should_die(t_philosopher *phil)
{
	if (phil->dead)
		return (1);
	if (get_time_in_us() - phil->prev_meal > phil->ttd)
	{
		phil->dead = 1;
		phrint(phil, DIE);
		return (1);
	}
	return (0);
}
