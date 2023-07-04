/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:18:03 by tsankola          #+#    #+#             */
/*   Updated: 2023/07/04 18:18:04 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# include <pthread.h>
# define SLEEP_CYCLE 250	// sleeping period in microseconds

enum e_arg_indices
{
	no_of_phils = 0,
	ttd = 1,
	tte = 2,
	tts = 3,
	max_meals = 4
};

enum e_mutex_indices
{
	DEATH_MUTEX_I = 0,
	PRINT_MUTEX_I = 1
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
	int				id;
	int				ttd;
	int				tte;
	int				tts;
	int				mm;
	int				eat_count;
	int				dead;
	long int		prev_meal;
	long int		inception;
	t_fork			*r_utensil;
	t_fork			*l_utensil;
	volatile int	*death;
	pthread_mutex_t	*mutexes;
}	t_philosopher;

long int	get_time_in_us();

long int	get_time_in_ms();

int			parse_args(int argc, char *argv[], unsigned int args[]);

void		eat(t_philosopher *phil);

void		think(t_philosopher *phil);

void		deep_think(t_philosopher *phil);

int			take_fork(t_philosopher *phil, t_fork *f);

void		drop_fork(t_fork *f);

int			should_die(t_philosopher *phil);

#endif
