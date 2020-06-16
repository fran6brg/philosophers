/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:40:11 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/16 19:07:42 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void		ft_loop_usleep(unsigned int n)
{
	uint64_t	start_time;

	start_time = get_time();
	while (1)
	{
		if (get_time() - start_time >= n)
			break ;
		usleep(50);
	}
}

/*
** On lock 2 fourchettes pour pouvoir manger, si indisponible on attend
** Sachant que les fourchettes sont au centre et non entre chaque philosophe
** struct timespec ts;
** ts.tv_sec = 0;
** ts.tv_nsec = g_banquet.time_to_sleep * 1000000;
** nanosleep(&ts, NULL);

** Usleep à la fin car si 2 philos attendent, alors de cette maniere le philo
** qui attendait prendra la fourchette avant le philosopher qui vient
** tout juste de finir de manger
*/

void			eat_sleep_think(t_philo *p)
{
	pthread_mutex_lock(&g_banquet.forks[p->pos % 2 ? p->rfork : p->lfork]);
	print_status(p, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(&g_banquet.forks[p->pos % 2 ? p->lfork : p->rfork]);
	print_status(p, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(&p->eating);

	p->last_meal = get_time();
	p->death_time = p->last_meal + g_banquet.time_to_die;
	print_status(p, IS_EATING);
	p->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);

	pthread_mutex_unlock(&g_banquet.forks[p->pos % 2 ? p->rfork : p->lfork]);
	pthread_mutex_unlock(&g_banquet.forks[p->pos % 2 ? p->lfork : p->rfork]);
	pthread_mutex_unlock(&p->eating);
	pthread_mutex_unlock(&p->eat_counter);

	print_status(p, IS_SLEEPING);
	ft_loop_usleep(g_banquet.time_to_sleep);
	print_status(p, IS_SLEEPING);

	usleep(100);
}
