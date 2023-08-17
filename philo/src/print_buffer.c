/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 10:55:56 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/15 10:56:00 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#include <stdlib.h>
#include <stdio.h>

int	print_buffer_init(t_print_buffer **pb, int size)
{
	if (pb == NULL)
		return (0);
	if (*pb == NULL)
	{
		*pb = malloc(sizeof(t_print_buffer));
		if (*pb == NULL)
			return (0);
		(*pb)->length = size * PRINT_ENTRIES_PER_PHILOSOPHER;
		(*pb)->buffer = malloc(sizeof(t_print_entry) * (*pb)->length);
		if ((*pb)->buffer == NULL)
		{
			free(*pb);
			*pb = NULL;
			return (0);
		}
	}
	(*pb)->last = 0;
	return (1);
}

int	print_buffer_del(t_print_buffer **pb)
{
	if (pb && *pb)
	{
		free((t_print_entry *)(*pb)->buffer);
		(*pb)->buffer = NULL;
		(*pb)->last = 0;
		(*pb)->length = 0;
		free(*pb);
		*pb = NULL;
		return (1);
	}
	return (0);
}

int	print_buffer_write(t_print_buffer volatile *pb, long int time, int id,
	int print_case)
{
	if (pb == NULL || pb->last == pb->length)
		return (0);
	pb->buffer[pb->last].time = time;
	pb->buffer[pb->last].id = id;
	pb->buffer[pb->last].print_case = print_case;
	pb->last++;
	return (1);
}

int	print_buffer_printf(t_print_buffer *pb)
{
	int	i;

	i = -1;
	while (++i < pb->last)
	{
		if (pb->buffer[i].print_case == EAT)
			printf("%ld %d is eating\n", pb->buffer[i].time, pb->buffer[i].id);
		else if (pb->buffer[i].print_case == SLEEP)
			printf("%ld %d is sleeping\n", pb->buffer[i].time,
				pb->buffer[i].id);
		else if (pb->buffer[i].print_case == FORK_TAKE)
			printf("%ld %d has taken a fork\n", pb->buffer[i].time,
				pb->buffer[i].id);
		else if (pb->buffer[i].print_case == THINK)
			printf("%ld %d is thinking\n", pb->buffer[i].time,
				pb->buffer[i].id);
		else if (pb->buffer[i].print_case == DIE)
		{
			printf("%ld %d died\n", pb->buffer[i].time, pb->buffer[i].id);
			return (0);
		}
	}
	return (1);
}
