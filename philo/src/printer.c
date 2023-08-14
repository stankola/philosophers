#include "philo.h"

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int	print_buffer_init(t_print_buffer **print_buffer, int size)
{
	if (print_buffer == NULL)
		return (0);
	if (*print_buffer == NULL)
	{
		*print_buffer = malloc(sizeof(t_print_buffer));
		if (*print_buffer == NULL)
			return (0);
		(*print_buffer)->length = size * PRINT_ENTRIES_PER_PHILOSOPHER;
		(*print_buffer)->buffer = malloc(sizeof(t_philo_print_entry) *
			(*print_buffer)->length);
		if ((*print_buffer)->buffer == NULL)
		{
			free(*print_buffer);
			*print_buffer = NULL;
			return (0);
		}
	}
	(*print_buffer)->last = 0;
	return (1);
}

int	print_buffer_write(volatile t_print_buffer *print_buffer, long int time, int id, int print_case)
{
	if (print_buffer == NULL || print_buffer->last == print_buffer->length)
	{
		fprintf(stderr, "Print buffer full! or Null ptr offered\n"); // TODO Delete this for eval
		return (0);
	}
	print_buffer->buffer[print_buffer->last].time = time;
	print_buffer->buffer[print_buffer->last].id = id;
	print_buffer->buffer[print_buffer->last].print_case = print_case;
	print_buffer->last++;
	return (1);
}

int	print_buffer_printf(t_print_buffer *print_buffer)
{
	int	i;

	i = -1;
	while (++i < print_buffer->last)
	{
		if (print_buffer->buffer[i].print_case == EAT)
			printf("%ld %d is eating\n", print_buffer->buffer[i].time,
				print_buffer->buffer[i].id); 
		else if (print_buffer->buffer[i].print_case == SLEEP)
			printf("%ld %d is sleeping\n", print_buffer->buffer[i].time,
				print_buffer->buffer[i].id); 
		else if (print_buffer->buffer[i].print_case == FORK_TAKE)
			printf("%ld %d has taken a fork\n", print_buffer->buffer[i].time,
				print_buffer->buffer[i].id); 
		else if (print_buffer->buffer[i].print_case == THINK)
			printf("%ld %d is thinking\n", print_buffer->buffer[i].time,
				print_buffer->buffer[i].id); 
		else if (print_buffer->buffer[i].print_case == DIE)
		{
			printf("%ld %d died\n", print_buffer->buffer[i].time,
				print_buffer->buffer[i].id);
			return (0);
		}
	}
	return (1);
}

void	*printer_thread(t_printer_thread *pr_thread)
{
	unsigned int	buffer_i;

	buffer_i = 0;
	pr_thread->buffer = pr_thread->buffers[buffer_i];
	while (1)
	{
		usleep(PRINT_INTERVAL);
		pthread_mutex_lock(pr_thread->print_mutex);
		pr_thread->buffer = pr_thread->buffers[!buffer_i];
		pthread_mutex_unlock(pr_thread->print_mutex);
		if (print_buffer_printf((t_print_buffer *)pr_thread->buffers[buffer_i]) == 0)
			return (NULL);
		pr_thread->buffers[buffer_i]->last = 0;
		buffer_i = !buffer_i;
	}
	return (NULL); // TODO return value
}

void	printer_thread_init(t_printer_thread **pr_thread, int size, pthread_mutex_t *print_mutex)
{
	if (pr_thread && *pr_thread == NULL)
		*pr_thread = malloc(sizeof(t_printer_thread));
	(*pr_thread)->buffers[0] = NULL;
	(*pr_thread)->buffers[1] = NULL;
	print_buffer_init((t_print_buffer **)&(*pr_thread)->buffers[0], size);
	fprintf(stderr, "Init %p 0\n", (*pr_thread)->buffers[0]);
	print_buffer_init((t_print_buffer **)&(*pr_thread)->buffers[1], size);
	fprintf(stderr, "Init %p 1\n", (*pr_thread)->buffers[1]);
	(*pr_thread)->buffer = (*pr_thread)->buffers[0];
	(*pr_thread)->print_mutex = print_mutex;
}
/*
#include <pthread.h>
int main(int argc, char *argv[])
{
	pthread_mutex_t	*mutex;
	t_printer_thread *pr_thread;
	pthread_t	thread;
	volatile t_print_buffer	** volatile bfr;

	mutex = malloc(sizeof(pthread_mutex_t));
	pthread_mutex_init(mutex, NULL);
	argv+=0;

	pr_thread = NULL;
	printer_thread_init(&pr_thread, 2, mutex);
	fprintf(stderr, "mutex %p\n", mutex);
	bfr =(volatile t_print_buffer **) &pr_thread->buffer;
	fprintf(stderr, "mutex %p\n", pr_thread->print_mutex);

	printf("starting thread\n");
	getchar();
	thread = pthread_create(&thread, NULL, (void *(*)(void *))printer_thread, pr_thread);

	for (int j = 0; j < 5; j++)
	{
		printf("%d.\n", j);
		for (int i = 1; i < argc / 2; i++)
		{
			pthread_mutex_lock(mutex);
			print_buffer_write((t_print_buffer *)*bfr, 123, i, 1);
			pthread_mutex_unlock(mutex);
		}
		printf("--\n");
		usleep(1200000);
	}
	printf("Ending\n");
	getchar();
	return (0);
}*/
