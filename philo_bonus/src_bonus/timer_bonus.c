/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timer_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/27 18:29:18 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/27 18:29:27 by tsankola         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <sys/time.h>
#include "philo_bonus.h"

long int	get_time_in_us()
{
	struct timeval	now;

	gettimeofday(&now, NULL);	// Might fail
	return (now.tv_sec * 1000000 + now.tv_usec);
}

long int	get_time_in_ms()
{
	struct timeval	now;

	gettimeofday(&now, NULL);	// Might fail
	return (now.tv_sec * 1000 + now.tv_usec / 1000);
}
