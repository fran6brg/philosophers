/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henri <henri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/28 15:38:15 by henri             #+#    #+#             */
/*   Updated: 2020/06/07 15:24:02 by henri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			printstatus(t_philo *philo, char *str)
{
	static int	x;

	pthread_mutex_lock(&g_context.mutexwrite);
	if (x == 0)
	{
		putuint64_t(1, chrono() - g_context.timer);
		write(1, "\t", 1);
		if (str[0] == 'm')
		{
			write(1, "max eat reached\n", 16);
			x = 1;
			pthread_mutex_unlock(&g_context.mutexwrite);
			return ;
		}
		putuint64_t(1, ((uint64_t)philo->pos + 1));
		write(1, " ", 1);
		putstrfd(str, 1);
		write(1, "\n", 1);
		if (str[0] == 'd')
			x = 1;
	}
	pthread_mutex_unlock(&g_context.mutexwrite);
}

/*
** On lock 2 fourchettes pour pouvoir manger, si indisponible on attend
** Sachant que les fourchettes sont au centre et non entre chaque philosophe
** struct timespec ts;
** ts.tv_sec = 0;
** ts.tv_nsec = g_context.time_to_sleep * 1000000;
** nanosleep(&ts, NULL);
*/

void			lock2forks(t_philo *philo)
{
	if ((philo->pos + 1) % 2 == 0)
	{
		pthread_mutex_lock(&g_context.mutexforks[philo->rfork]);
		printstatus(philo, "has taken a fork");
		pthread_mutex_lock(&g_context.mutexforks[philo->lfork]);
		printstatus(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&g_context.mutexforks[philo->lfork]);
		printstatus(philo, "has taken a fork");
		pthread_mutex_lock(&g_context.mutexforks[philo->rfork]);
		printstatus(philo, "has taken a fork");
	}
}

/*
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

/*
** On ne multiplie pas par 1000 le time_to_die dans l'appel de la fonction !
** printf("---> %u \n", n);
** printf("1) PHILO#%d   ---> %" PRIu64 "\n", pos, chrono());
** usleep(n);
** printf("1) PHILO#%d   ---> %" PRIu64 "\n", pos, chrono());
**
** struct timeval timeBef;
** struct timeval timeAft;
** gettimeofday(&timeBef, 0);
** gettimeofday(&timeAft, 0);
** printf("TIME GAP = %d\n", (timeAft.tv_usec - timeBef.tv_usec) / 1000);
*/

static void		ft_usleep(unsigned int n)
{
	uint64_t	start;

	start = chrono();
	while (1)
	{
		usleep(50);
		if (chrono() - start >= n)
			break ;
	}
}

/*
** Usleep à la fin car si 2 philos attendent, alors de cette maniere le philo
** qui attendait prendra la fourchette avant le philosopher qui vient
** tout juste de finir de manger
*/

void			eat(t_philo *philo)
{
	lock2forks(philo);
	pthread_mutex_lock(&philo->philomutex);
	philo->last_meal = chrono();
	philo->remainingtime = philo->last_meal + g_context.time_to_die;
	printstatus(philo, "is eating");
	philo->meal_count += 1;
	ft_usleep(g_context.time_to_eat);
	pthread_mutex_unlock(&g_context.mutexforks[philo->lfork]);
	pthread_mutex_unlock(&g_context.mutexforks[philo->rfork]);
	pthread_mutex_unlock(&philo->philomutex);
	pthread_mutex_unlock(&philo->philomutexeatcount);
	printstatus(philo, "is sleeping");
	ft_usleep(g_context.time_to_sleep);
	printstatus(philo, "is thinking");
	if ((philo->pos + 1) % 2 == 0)
		usleep(500);
}
