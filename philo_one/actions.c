/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:40:11 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/16 16:13:59 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** On lock 2 fourchettes pour pouvoir manger, si indisponible on attend
** Sachant que les fourchettes sont au centre et non entre chaque philosophe
** struct timespec ts;
** ts.tv_sec = 0;
** ts.tv_nsec = g_banquet.time_to_sleep * 1000000;
** nanosleep(&ts, NULL);
*/

void			lock2forks(t_philo *philo)
{
	if (philo->pos % 2 == 0)
	{
		pthread_mutex_lock(&g_banquet.forks[philo->lfork]);
		print_status(philo, HAS_TAKEN_A_FORK);
		pthread_mutex_lock(&g_banquet.forks[philo->rfork]);
		print_status(philo, HAS_TAKEN_A_FORK);
	}
	else
	{
		pthread_mutex_lock(&g_banquet.forks[philo->rfork]);
		print_status(philo, HAS_TAKEN_A_FORK);
		pthread_mutex_lock(&g_banquet.forks[philo->lfork]);
		print_status(philo, HAS_TAKEN_A_FORK);
	}
}

/*
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

/*
** On ne multiplie pas par 1000 le time_to_die dans l'appel de la fonction !
** printf("---> %u \n", n);
** printf("1) PHILO#%d   ---> %" PRIu64 "\n", pos, get_time());
** usleep(n);
** printf("1) PHILO#%d   ---> %" PRIu64 "\n", pos, get_time());
**
** struct timeval timeBef;
** struct timeval timeAft;
** gettimeofday(&timeBef, 0);
** gettimeofday(&timeAft, 0);
** printf("TIME GAP = %d\n", (timeAft.tv_usec - timeBef.tv_usec) / 1000);
*/

void		ft_usleep(unsigned int n)
{
	uint64_t	start;

	start = get_time();
	while (1)
	{
		usleep(50);
		if (get_time() - start >= n)
			break ;
	}
}

/*
** Usleep à la fin car si 2 philos attendent, alors de cette maniere le philo
** qui attendait prendra la fourchette avant le philosopher qui vient
** tout juste de finir de manger
*/

void			eat_sleep_think(t_philo *philo)
{
	lock2forks(philo);
	
	pthread_mutex_lock(&philo->eating);
	philo->last_meal = get_time();
	philo->death_time = philo->last_meal + g_banquet.time_to_die;
	print_status(philo, IS_EATING);
	philo->meal_count += 1;
	ft_usleep(g_banquet.time_to_eat);
	pthread_mutex_unlock(&g_banquet.forks[philo->lfork]);
	pthread_mutex_unlock(&g_banquet.forks[philo->rfork]);
	pthread_mutex_unlock(&philo->eating);
	pthread_mutex_unlock(&philo->eat_counter);

	print_status(philo, IS_SLEEPING);
	ft_usleep(g_banquet.time_to_sleep);

	print_status(philo, IS_SLEEPING);
	usleep(500);
}
