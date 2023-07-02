/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 16:54:55 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/27 18:30:03 by tsankola         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <limits.h>
#include "philo_bonus.h"

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

static int	get_value_uint(char *s, unsigned int *value)
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
	if (!check_form_uint(s) || !get_value_uint(s, value))
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
		if (! atoui(argv[i + 1], &args[i]))	//TODO sanity check, what if tts + tte > ttd ??
			return (22);
		i++;
	}
	return (0);
}
