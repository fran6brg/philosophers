/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:40 by user42            #+#    #+#             */
/*   Updated: 2020/06/21 02:10:42 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			ft_loop_usleep(unsigned int n)
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
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

int				eat_sleep_think(t_philo *philo)
{
	if (sem_wait(g_banquet.ask_forks)
	|| sem_wait(g_banquet.forks)
	|| print_status(philo, HAS_TAKEN_A_FORK)
	|| sem_wait(g_banquet.forks)
	|| print_status(philo, HAS_TAKEN_A_FORK)
	|| sem_post(g_banquet.ask_forks)
	|| sem_wait(philo->philosema))
		return (RET_ERROR);
	philo->last_meal = get_time();
	philo->remainingtime = philo->last_meal + g_banquet.time_to_die;
	if (print_status(philo, IS_EATING))
		return (RET_ERROR);
	philo->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);
	if (sem_post(philo->philosema)
	|| sem_post(philo->philo_eat_count)
	|| sem_post(g_banquet.forks)
	|| sem_post(g_banquet.forks)
	|| print_status(philo, IS_SLEEPING))
		return (RET_ERROR);
	ft_loop_usleep(g_banquet.time_to_sleep);
	if (print_status(philo, IS_THINKING))
		return (RET_ERROR);
	return (RET_SUCCESS);
}
