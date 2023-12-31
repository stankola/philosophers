/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printer_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 10:57:02 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/21 10:15:29 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include "philo_bonus.h"

void	phrint(t_philosopher *phil, int print_case)
{
	long	time;

	time = get_time_in_ms() - phil->inception;
	sem_wait(phil->stenographer.buffer_sem);
	print_buffer_write(phil->stenographer.buffer, time, phil->id, print_case);
	sem_post(phil->stenographer.buffer_sem);
}

void	printer_thread_stop(t_printer_thread *pt)
{
	sem_wait(pt->stop_sem);
	pt->stop = 1;
	sem_post(pt->stop_sem);
	return ;
}

int	printer_thread_del(t_printer_thread *pt)
{
	if (pt)
	{
		pt->buffer = NULL;
		sem_close(pt->print_sem);
		sem_close(pt->stop_sem);
		sem_unlink(pt->stop_sem_name);
		sem_close(pt->buffer_sem);
		sem_unlink(pt->buffer_sem_name);
		return (1);
	}
	return (0);
}

void	*printer_thread(t_printer_thread *pt)
{
	unsigned int	buffer_i;
	int				ret_val;

	buffer_i = 0;
	sem_wait(pt->stop_sem);
	while (pt->stop == 0)
	{
		sem_post(pt->stop_sem);
		usleep(PRINT_INTERVAL);
		sem_wait(pt->buffer_sem);
		pt->buffer = &pt->buffers[!buffer_i];
		sem_post(pt->buffer_sem);
		sem_wait(pt->print_sem);
		ret_val = print_buffer_printf(&pt->buffers[buffer_i]);
		if (ret_val == 0)
			return (NULL);
		sem_post(pt->print_sem);
		pt->buffers[buffer_i].last = 0;
		buffer_i = !buffer_i;
		sem_wait(pt->stop_sem);
	}
	sem_post(pt->stop_sem);
	return (NULL);
}

int	printer_thread_init(t_printer_thread *pt, int id)
{
	if (pt == NULL)
		return (0);
	pt->id = id;
	print_buffer_init(&pt->buffers[0]);
	print_buffer_init(&pt->buffers[1]);
	pt->buffer = &pt->buffers[0];
	pt->print_sem = sem_open(PRINT_SEM_NAME, O_RDWR);
	name_generator(pt->buffer_sem_name, BUFFER_SEM_NAME, id);
	unlink(pt->buffer_sem_name);
	pt->buffer_sem = sem_open(pt->buffer_sem_name, O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	pt->stop = 0;
	name_generator(pt->stop_sem_name, PRINTER_STOP_SEM, id);
	unlink(pt->stop_sem_name);
	pt->stop_sem = sem_open(pt->stop_sem_name, O_RDWR | O_CREAT,
			S_IRUSR | S_IWUSR, 1);
	return (1);
}