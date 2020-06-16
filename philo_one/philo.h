/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:40:29 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/16 17:07:58 by francisberg      ###   ########.fr       */
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

# define RET_SUCCESS		0
# define RET_ERROR			1

# define HAS_TAKEN_A_FORK	2
# define IS_EATING			3
# define IS_SLEEPING		4
# define IS_THINKING		5
# define HAS_LEFT_ITS_FORKS	6
# define MAX_EAT_REACHED	7
# define DIED				8

/*
** philosophers = nombre de philosophes et de fourchettes
** number_of_meal = nombre de fois qu'un philosophe doit manger (optionnel)
** lfork = fourchette gauche
** Pour toucher aux variables on lock d'abord le mutex mutex
** Idem mutex_eat_boolean
*/

typedef struct		s_philo
{
	int				pos;
	int				meal_count;
	int				lfork;
	int				rfork;
	uint64_t		last_meal;
	uint64_t		death_time;
	pthread_mutex_t	eating;
	pthread_mutex_t	eat_counter;
}					t_philo;

typedef struct		s_banquet
{
	int				nb_philos;
	int				max_eat;
	uint64_t		start_time;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	pthread_mutex_t	stop_banquet;
	pthread_mutex_t	write;
}					t_banquet;

t_banquet			g_banquet;

/*
** main.c
*/

void				*handle_max_eat(void *arg);
void				*handle_death(void *philo_uncasted);
void				*philo_life(void *philo_uncasted);
int					start_banquet(void);
int					main(int ac, char **av);

/*
** init.c
*/

int					set_philos(void);
int					set_mutex(void);
int					check_config(void);
int					parse_banquet_config(int ac, char **av);

/*
** actions.c
*/

void				lock2forks(t_philo *philo);
void				ft_usleep(unsigned int n);
void				eat_sleep_think(t_philo *philo);

/*
** logs.c
*/

void				add_nb_to_log(char *buf, int *index, uint64_t n);
void				add_str_to_log(char *buf, int *i, char *str);
void				add_status_to_log(char *log, int *i, const int status);
void				print_status(t_philo *philo, const int status);

/*
** utils.c
*/

int					ft_strcomp(char *s1, char *s2);
uint64_t			get_time(void);
void				ft_putstrfd(char *str, int fd);
int					ft_atoi(char *str);

/*
** end.c
*/

int 				ft_printerror(char *msg, int clean);
int					ft_clean(void);

#endif
