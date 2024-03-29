/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:08:26 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:43:30 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void		eat_sleep_think(t_philo *p)
{
	pthread_mutex_lock(&g_banquet.forks[p->pos % 2 ? p->rfork : p->lfork]);
	print_log(p, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(&g_banquet.forks[p->pos % 2 ? p->lfork : p->rfork]);
	print_log(p, HAS_TAKEN_A_FORK);
	pthread_mutex_lock(&p->eating);
	p->last_meal = get_time();
	p->death_time = p->last_meal + g_banquet.time_to_die;
	print_log(p, IS_EATING);
	p->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);
	pthread_mutex_unlock(&g_banquet.forks[p->pos % 2 ? p->rfork : p->lfork]);
	pthread_mutex_unlock(&g_banquet.forks[p->pos % 2 ? p->lfork : p->rfork]);
	pthread_mutex_unlock(&p->eating);
	pthread_mutex_unlock(&p->eat_counter);
	print_log(p, IS_SLEEPING);
	ft_loop_usleep(g_banquet.time_to_sleep);
	print_log(p, IS_SLEEPING);
	usleep(100);
}
