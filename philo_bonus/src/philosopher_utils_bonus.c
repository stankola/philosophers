/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 15:18:23 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/21 10:15:46 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "philo_bonus.h"

static int	ph_pow(int x, int y)
{
	if (y == 0)
		return (1);
	while (y-- > 1)
		x *= x;
	return (x);
}

// Not actually used anywhere. Retained just in case.
// int overflow in while loops possible
char	*ph_itoa(int i)
{
	char	*a;
	int		degree;
	int		j;

	degree = 1;
	while (i / (ph_pow(10, degree)) > 0)
		degree++;
	a = malloc(sizeof(char) * (degree + 1));
	if (a == NULL)
		return (NULL);
	j = -1;
	while (++j < degree)
		a[j] = i / ph_pow(10, degree - j) % 10;
	a[j] = '\0';
	return (a);
}

// int might overflow in the loop but shouldn't be a problem within this
// project's scope
static int	ilen(int i)
{
	int	l;

	l = 1;
	while (i / (ph_pow(10, l)) > 0)
		l++;
	return (l);
}

void	name_generator(char *buffer, char *prefix, int suffix)
{
	int		suf_pos;
	int		i;
	int		j;

	i = -1;
	while (prefix[++i] != '\0')
		buffer[i] = prefix[i];
	suf_pos = ilen(suffix);
	j = -1;
	while (++j < ilen(suffix))
		buffer[i++] = '0' + suffix / ph_pow(10, --suf_pos) % 10;
	buffer[i] = '\0';
}
