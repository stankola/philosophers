#include "philo.h"

#include <unistd.h>
#include <stdlib.h>

void	printer_thread_stop(t_printer_thread *pt)
{
	pthread_mutex_lock(&pt->stop_mutex);
	pt->stop = 1;
	pthread_mutex_unlock(&pt->stop_mutex);
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
		pthread_mutex_destroy(&(*pt)->stop_mutex);
		(*pt)->print_mutex = NULL;
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
	pthread_mutex_lock(&pt->stop_mutex);
	while (pt->stop == 0)
	{
		pthread_mutex_unlock(&pt->stop_mutex);
		usleep(PRINT_INTERVAL);
		pthread_mutex_lock(pt->print_mutex);
		pt->buffer = pt->buffers[!buffer_i];
		pthread_mutex_unlock(pt->print_mutex);
		if (print_buffer_printf((t_print_buffer *)pt->buffers[buffer_i]) == 0)
			return (NULL);
		pt->buffers[buffer_i]->last = 0;
		buffer_i = !buffer_i;
		pthread_mutex_lock(&pt->stop_mutex);
	}
	pthread_mutex_unlock(&pt->stop_mutex);
	return (NULL);
}

int	printer_thread_init(t_printer_thread **pt, int size, pthread_mutex_t *pm)
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
	(*pt)->print_mutex = pm;
	(*pt)->stop = 0;
	pthread_mutex_init(&(*pt)->stop_mutex, NULL);
	return (1);
}

