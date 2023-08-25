/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 10:57:02 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/25 17:33:17 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <unistd.h>
#include <stdlib.h>

void	printer_thread_stop(t_printer_thread *pt)
{
	sem_t	*sem;

	sem = sem_open(PRINTER_STOP_SEM, O_RDWR);
	sem_wait(sem);
	pt->stop = 1;
	sem_post(sem);
	return ;
}

int	printer_thread_del(t_printer_thread **pt)
{
	if (pt && *pt)
	{
		print_buffer_del((t_print_buffer **)&(*pt)->buffers[0]);
		print_buffer_del((t_print_buffer **)&(*pt)->buffers[1]);
		free((t_print_buffer *)*(*pt)->buffers);
		(*pt)->buffer = NULL;
		sem_close((*pt)->stop_sem);
		sem_unlink(PRINTER_STOP_SEM);
		(*pt)->print_sem = NULL;
		free(*pt);
		*pt = NULL;
		return (1);
	}
	return (0);
}

void	*printer_thread(t_printer_thread *pt)
{
	unsigned int	buffer_i;

	buffer_i = 0;
	pt->buffer = pt->buffers[buffer_i];
	sem_wait(&pt->stop_sem);
	while (pt->stop == 0)
	{
		sem_post(&pt->stop_sem);
		usleep(PRINT_INTERVAL);
		sem_wait(pt->print_sem);
		pt->buffer = pt->buffers[!buffer_i];
		sem_post(pt->print_sem);
		if (print_buffer_printf((t_print_buffer *)pt->buffers[buffer_i]) == 0)
			return (NULL);
		pt->buffers[buffer_i]->last = 0;
		buffer_i = !buffer_i;
		sem_wait(&pt->stop_sem);
	}
	sem_post(&pt->stop_sem);
	return (NULL);
}

int	printer_thread_init(t_printer_thread **pt, int size, sem_t *ps)
{
	if (pt && *pt == NULL)
	{
		*pt = malloc(sizeof(t_printer_thread));
		if (*pt == NULL)
			return (0);
	}
	(*pt)->buffers[0] = NULL;
	(*pt)->buffers[1] = NULL;
	print_buffer_init((t_print_buffer **)&(*pt)->buffers[0], size);
	print_buffer_init((t_print_buffer **)&(*pt)->buffers[1], size);
	(*pt)->buffer = (*pt)->buffers[0];
	(*pt)->print_sem = sem_open(PRINT_SEM, O_RDWR);
	(*pt)->stop = 0;
	(*pt)->stop_sem = sem_open(PRINTER_STOP_SEM, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 1);
	return (1);
}
