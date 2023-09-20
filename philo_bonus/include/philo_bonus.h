/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsankola <tsankola@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/04 18:18:03 by tsankola          #+#    #+#             */
/*   Updated: 2023/09/20 04:31:27 by tsankola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <semaphore.h>
# include <pthread.h>
# include <sys/types.h>
# define LONG_SLEEP 10000
# define MEDIUM_SLEEP 2500
# define SNOOZE 500
# define PRINT_INTERVAL 1000
# define PRINT_ENTRIES_PER_PHILOSOPHER 32
# define EXIT_STARVED 1
# define EXIT_SATIATED 0
# define PRINT_SEM_NAME "Print"
# define BUFFER_SEM_NAME "Printer_buffer"
# define PRINTER_STOP_SEM "Printer_stop"
# define DEATH_SEM_NAME "Exitus"
# define FORK_SEM_NAME "Utensils"
# define FORK2_SEM_NAME "Utensils2"

enum e_arg_indices
{
	no_of_phils = 0,
	ttd = 1,
	tte = 2,
	tts = 3,
	max_meals = 4
};

typedef enum e_print_cases
{
	EAT,
	SLEEP,
	FORK_TAKE,
	DIE,
	THINK
}	t_print_case;

typedef enum e_sem_name_cases
{
	PRINTER,
	STOP
}	t_sem_name_case;

typedef struct s_print_entry
{
	long int	time;
	int			id;
	int			print_case;
}	t_print_entry;

typedef struct s_print_buffer
{
	volatile t_print_entry	*buffer;
	volatile int			last;
	volatile int			length;
}	t_print_buffer;

typedef struct s_printer_thread
{
	t_print_buffer volatile	*buffers[2];
	t_print_buffer volatile	*buffer;
	sem_t					*print_sem;
	sem_t					*stop_sem;
	char					*stop_sem_name;
	sem_t					*buffer_sem;
	char					*buffer_sem_name;
	int volatile			stop;
	int						id;
}	t_printer_thread;

typedef struct s_philosopher
{
	pid_t				pid;
	int					id;
	int					ttd;
	int					tte;
	int					tts;
	int					mm;
	int					eat_count;
	int					dead;
	long int			prev_meal;
	int					no_of_phils;
	long int			inception;
	t_printer_thread	*stenographer;
	sem_t				*utensil_pairs;
	sem_t				*utensils;
}	t_philosopher;

long int	get_time_in_us(void);

long int	get_time_in_ms(void);

int			get_time_to_meal(t_philosopher *phil);

int			parse_args(int argc, char *argv[], unsigned int args[]);

void		eat(t_philosopher *phil);

void		think(t_philosopher *phil);

void		deep_think(t_philosopher *phil);

int			take_forks(t_philosopher *phil);

void		drop_forks(t_philosopher *phil);

void		phleep(t_philosopher *phil, unsigned int duration);

void		phacilitate(t_philosopher *phils, int philc);

int			should_die(t_philosopher *phil);

void		phrint(t_philosopher *phil, int print_case);

int			print_buffer_init(t_print_buffer **pb, int size);

int			print_buffer_del(t_print_buffer **pb);

int			print_buffer_write(volatile t_print_buffer *pb, long int time,
				int id, int print_case);

int			print_buffer_printf(t_print_buffer *pb);

void		printer_thread_stop(t_printer_thread *pt);

void		*printer_thread(t_printer_thread *pt);

int			printer_thread_init(t_printer_thread **pt, int size, int id);

int			printer_thread_del(t_printer_thread **pt);

char		*name_generator(char *prefix, int suffix);

#endif
