/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_buffer_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 10:55:56 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/21 10:16:14 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include "philo_bonus.h"

int	print_buffer_init(t_print_buffer *pb)
{
	if (pb == NULL)
		return (0);
	pb->length = PRINT_ENTRIES_PER_PHILOSOPHER;
	pb->last = 0;
	return (1);
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
