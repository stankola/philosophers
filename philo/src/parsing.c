/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:54:55 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/16 16:54:56 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <limits.h>
#include "philo.h"

static int	check_form_uint(char *s)
{
	if (*s == '\0')
		return (0);
	while (*s >= '0' && *s <= '9')
		s++;
	if (*s == '\0')
		return (1);
	return (0);
}

static int	check_value_uint(char *s, unsigned int *value)
{
	*value = 0;
	while (*s != '\0')
	{
		if (*value > 0 && *value > UINT_MAX / 10)
			return (0);
		*value *= 10;
		if (*value > 0 && *value > UINT_MAX - (*s - '0'))
			return (0);
		*value += (*s - '0');
		s++;
	}
	return (1);
}

static int	atoui(char *s, unsigned int *value)
{
	if (!check_form_uint(s) || !check_value_uint(s, value))
		return (0);
	return (1);
}

int	parse_args(int argc, char *argv[], unsigned int args[])
{
	int	i;

	if (argc != 5 && argc != 6)
		return (22);
	i = 0;
	while (i < argc - 1)
	{
		if (! atoui(argv[i + 1], &args[i]))
			return (22);
		i++;
	}
	return (0);
}
