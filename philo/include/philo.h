/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/16 14:26:41 by tsankola          #+#    #+#             */
/*   Updated: 2023/06/16 14:26:42 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# include <sys/time.h>
# define SLEEP_CYCLE 50	// waiting increment size in microseconds
# define DYING_THRESHOLD 300 // Threshold in microseconds that a philosopher is allowed to go over his time to die

/* Arguments:
number_of_philosophers
time_to_die
time_to_eat
time_to_sleep
[number_of_times_each_philosopher_must_eat]
Let valid values belong to the range [0,UINT_MAX]
*/
enum e_args
{
	no_of_phils = 0,
	time_to_die = 1,
	time_to_eat = 2,
	time_to_sleep = 3,
	max_meals = 4
};

enum e_mutex_indices
{
	DEATH_MUTEX = 0,
	PRINT_MUTEX = 1
};

enum e_print_cases
{
	EAT,
	SLEEP,
	FORK_TAKE,
	DIE,
	THINK
};

typedef struct s_fork
{
	volatile int	taken;
	pthread_mutex_t	grab_mutex;
	pthread_mutex_t	fork_mutex;
}	t_fork;

typedef struct s_philosopher
{
	int			id;
	int			ttd;
	int			tte;
	int			tts;
	int			mm;
	int			eat_count;
	int			dead;
	long int	prev_meal;
	long int	inception;
	t_fork		*r_utensil;
	t_fork		*l_utensil;
	pthread_mutex_t	*death_mutex;
//	pthread_mutex_t	*mutexes;
}	t_philosopher;

long int	get_time_in_us();

long int	get_time_in_ms();

int			parse_args(int argc, char *argv[], unsigned int args[]);

void		die(t_philosopher *phil);

void		eat(t_philosopher *phil);

void		think(t_philosopher *phil);

void		deep_think(t_philosopher *phil);

void		take_fork(t_philosopher *phil, t_fork *f);

void		drop_fork(t_fork *f);

#endif