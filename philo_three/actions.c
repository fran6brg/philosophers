/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:40 by user42            #+#    #+#             */
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

int				eat_sleep_think(t_philo *p)
{
	if (sem_wait(g_banquet.ask_forks)
	|| sem_wait(g_banquet.forks)
	|| print_log(p, HAS_TAKEN_A_FORK)
	|| sem_wait(g_banquet.forks)
	|| print_log(p, HAS_TAKEN_A_FORK)
	|| sem_post(g_banquet.ask_forks)
	|| sem_wait(p->eating))
		return (RET_ERROR);
	p->last_meal = get_time();
	p->death_time = p->last_meal + g_banquet.time_to_die;
	if (print_log(p, IS_EATING))
		return (RET_ERROR);
	p->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);
	if (sem_post(p->eating)
	|| sem_post(p->eat_count)
	|| sem_post(g_banquet.forks)
	|| sem_post(g_banquet.forks)
	|| print_log(p, IS_SLEEPING))
		return (RET_ERROR);
	ft_loop_usleep(g_banquet.time_to_sleep);
	if (print_log(p, IS_THINKING))
		return (RET_ERROR);
	return (RET_SUCCESS);
}
