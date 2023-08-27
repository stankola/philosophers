/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_utils_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/27 15:18:23 by tsankola          #+#    #+#             */
/*   Updated: 2023/08/27 15:35:20 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

#include <stdlib.h>

static int	ph_pow(int x, int y)
{
	if (y == 0)
		return (1);
	while (y-- > 1)
		x *= x;
	return (x);
}

static char	*ph_itoa(int i)
{
	char	*a;
	int		degree;
	int		j;

	degree = 1;
	while (i / (ph_pow(10, degree)) > 0)			// int overflow possible
		degree++;								// int overflow possible
	a = malloc(sizeof(char) * (degree + 1));
	if (a == NULL)
		return (NULL);
	j = -1;
	while (++j < degree)
		a[j] = i / ph_pow(10, degree - j) % 10;
	a[j] = '\0';
	return (a);
}

static int	ph_strlen(char *s)
{
	int	l;

	l = 0;
	while (*s++ != '\0')
		l++;
	return (l);
}

static int	ilen(int i)
{
	int	l;

	l = 1;
	while (i / (ph_pow(10, l)) > 0)			// int overflow possible
		l++;								// int overflow possible
	return (l);
}

char	*name_generator(char *prefix, int suffix)
{
	char	*name;
	int		s_l;
	int		i;

	s_l = ilen(suffix);
	name = malloc(sizeof(char) * (ph_strlen(prefix) + ilen(suffix) + 1));
	i = -1;
	while (++i < ph_strlen(prefix))
		name[i] = prefix[i];
	while (i < (ph_strlen(prefix) + ilen(suffix)))
		name[i++] = '0' + suffix / ph_pow(10, --s_l) % 10;
	name[i] = '\0';
	return (name);
}
