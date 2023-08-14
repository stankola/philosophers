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
# include <sys/types.h>
# define LONG_SLEEP_INTERVAL 10000
# define MEDIUM_SLEEP_INTERVAL 2500
# define SHORT_SLEEP_INTERVAL 500
# define PRINT_INTERVAL 1000
# define PRINT_ENTRIES_PER_PHILOSOPHER 8

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

typedef struct s_print_entry
{
	long int	time;
	int		id;
	int		print_case;
}	t_philo_print_entry;

typedef struct s_print_buffer
{
	volatile t_philo_print_entry	*buffer;
	volatile int	last;
	volatile int	length;
}	t_print_buffer;

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
	int				no_of_phils;
	long int		inception;
	t_fork			*r_utensil;
	t_fork			*l_utensil;
	volatile int	*death;
	pthread_mutex_t	*mutexes;
	volatile t_print_buffer	**print_buffer;
}	t_philosopher;

typedef struct s_printer_thread
{
	volatile t_print_buffer	*buffers[2];
	volatile t_print_buffer	* volatile buffer;
	pthread_mutex_t	*print_mutex;
}	t_printer_thread;

long int	get_time_in_us(void);

long int	get_time_in_ms(void);

int			parse_args(int argc, char *argv[], unsigned int args[]);

void		eat(t_philosopher *phil);

void		think(t_philosopher *phil);

void		deep_think(t_philosopher *phil);

int			take_fork(t_philosopher *phil, t_fork *f);

void		drop_fork(t_fork *f);

int			phleep(t_philosopher *phil, suseconds_t duration);

pthread_t	*phacilitate(t_philosopher *phils, int philc,
	t_printer_thread *pr_thread);

int			should_die(t_philosopher *phil);

void		phrint(int print_case, t_philosopher *phil);

int			print_buffer_init(t_print_buffer **print_buffer, int size);

int			print_buffer_write(volatile t_print_buffer *print_buffer,
	long int time, int id, int print_case);

int			print_buffer_printf(t_print_buffer *print_buffer);

void			*printer_thread(t_printer_thread *pr_thread);

void		printer_thread_init(t_printer_thread **pr_thread, int size,
	pthread_mutex_t *print_mutex);

#endif
