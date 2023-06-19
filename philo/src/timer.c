#include <stdlib.h>
#include <sys/time.h>
#include "philo.h"

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

long int	get_elapsed_time(long int since)
{
	return (get_time_in_ms() - since);
}