/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:41:09 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/21 01:52:09 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/time.h>
# include <inttypes.h>
# include <semaphore.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>

# define RET_SUCCESS		0
# define RET_ERROR			1

# define HAS_TAKEN_A_FORK	2
# define IS_EATING			3
# define IS_SLEEPING		4
# define IS_THINKING		5
# define HAS_LEFT_ITS_FORKS	6
# define MAX_EAT_REACHED	7
# define DIED				8

# define PHI_INIT			0
# define EAT_INIT			1

# define FORKS				"/FORKS"
# define WRITE				"/WRITE"
# define DEATH				"/DEATH"
# define PROCESSDEATH		"/PROCESSDEATH"
# define ASKTAKEFORKS		"/ASKTAKEFORKS"

typedef struct				s_philo
{
	int						pos;
	int						meal_count;
	uint64_t				last_meal;
	uint64_t				remainingtime;
	pid_t					pid;
	sem_t					*philosema;
	sem_t					*philo_eat_count;
}							t_philo;

typedef struct				s_banquet
{
	int						nb_philos;
	int						globaleatcoutner;
	int						max_eat;
	uint64_t				start_time;
	uint64_t				time_to_die;
	uint64_t				time_to_eat;
	uint64_t				time_to_sleep;
	t_philo					*philos;
	sem_t					*forks;
	sem_t					*ask_forks;
	sem_t					*death;
	sem_t					*write;
	sem_t					*process_death;
}							t_banquet;

t_banquet					g_banquet;

/*
** main.c
*/

void						*handle_max_eat(void *arg);
void						*handle_death(void *philo_uncasted);
int							philo_life(void *philo_uncasted);
int							start_banquet(void);
int							main(int ac, char **av);

/*
** init.c
*/

void						semanames(char *name, int id, int eat);
int							set_philos(void);
int							set_semas(int philonum);
int							check_config(void);
int							parse_banquet_config(int ac, char **av);

/*
** actions.c
*/

void						ft_loop_usleep(unsigned int n);
int							eat_sleep_think(t_philo *philo);

/*
** logs.c
*/

uint64_t					get_time(void);
void						add_str_to_log(char *log, int *i, char *str);
void						add_nb_to_log(char *log, int *i, uint64_t n,
							char end);
void						add_status_to_log(char *log, int *i,
							const int status);
int							print_status(t_philo *philo, const int status);

/*
** utils.c
*/

void						ft_putstrfd(char *str, int fd);
int							ft_atoi(char *str);

/*
** end.c
*/

int							ft_printerror(char *msg, int clean);
int							ft_clean(void);

#endif
