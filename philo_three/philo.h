/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henri <henri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/14 22:57:01 by henri             #+#    #+#             */
/*   Updated: 2020/06/06 22:24:12 by henri            ###   ########.fr       */
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

# define PHI_INIT		0
# define EAT_INIT		1

# define SEMAFORKS		"/SEMAFORKS"
# define SEMAWRITE		"/SEMAWRITE"
# define SEMADEATH		"/SEMADEATH"
# define SEMAPROCESSDEATH	"/SEMAPROCESSDEATH"
# define ASKTAKEFORKS	"/ASKTAKEFORKS"

typedef struct		s_philo
{
	int				pos;
	int				meal_count;
	uint64_t		last_meal;
	uint64_t		remainingtime;
	pid_t			philopid;
	sem_t			*philosema;
	sem_t			*philosemaeatcount;
}					t_philo;

/*
** Les variables globales sont entièrement dupliquées quand on fork()
** mais les sémaphores contenus dedans semblent être partagés
** The semaphore can be shared across threads or processes
** More infos here :
** https://stackoverflow.com/questions/6847973/do-forked-child-
** processes-use-the-same-semaphore
*/

typedef struct		s_context
{
	int				philosophers;
	int				globaleatcoutner;
	int				maxeat;
	uint64_t		timer;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	t_philo			*philos;
	sem_t			*semaforks;
	sem_t			*semaskforks;
	sem_t			*semadeath;
	sem_t			*semawrite;
	sem_t			*semaprocessdeath;
}					t_context;

t_context			g_context;

void				putstrfd(char *str, int fd);
int					ft_atoi(char *str);
int					initcontext(int ac, char **av);
void				putuint64_t(int fd, uint64_t nbr);
uint64_t			chrono(void);
int					strcompare(char *s1, char *s2);
int					printstatus(t_philo *philo, char *str);
int					lock2forks(t_philo *philo);
int					sleep_unlock2forks(t_philo *philo);
int					eat(t_philo *philo);
void				clear(void);
int					threadmaxeat(void);
void				*watchingmaxeat(void *arg);

#endif
