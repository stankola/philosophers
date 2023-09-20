/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_main_utils_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 05:00:44 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 05:17:07 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <semaphore.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "philo_bonus.h"

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
static void	stop(t_philosopher *phils, int i, pid_t	exclude_pid,
	sem_t *printsem)
{
	int		status;

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

	i = -1;
	printsem = NULL;
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
			if (!WIFSIGNALED(status) && WEXITSTATUS(status) == EXIT_STARVED)
				break ;
			else if (!WIFSIGNALED(status) && WEXITSTATUS(status) != EXIT_FULL)
				printsem = sem_open(PRINT_SEM_NAME, 0);
		}
	}
	stop(phils, i, exclude_pid, printsem);
}
