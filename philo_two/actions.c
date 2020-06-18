/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:41:36 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/18 17:03:59 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void		ft_loop_usleep(unsigned int n)
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
** On lock 2 fourchettes pour pouvoir manger, si indisponible on attend
** Sachant que les fourchettes sont au centre et non entre chaque philosophe
*/

int				sleep_think(t_philo *philo)
{
	if (print_status(philo, IS_SLEEPING))
		return (RET_ERROR);
	if (sem_post(g_banquet.forks))
		return (RET_ERROR);
	if (sem_post(g_banquet.forks))
		return (RET_ERROR);
	ft_loop_usleep(g_banquet.time_to_sleep);
	if (print_status(philo, IS_THINKING))
		return (RET_ERROR);
	usleep(500);
	return (RET_SUCCESS);
}

/*
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

int				eat(t_philo *philo)
{
	if (sem_wait(g_banquet.ask_forks))
		return (RET_ERROR);
	if (sem_wait(g_banquet.forks))
		return (RET_ERROR);
	if (print_status(philo, HAS_TAKEN_A_FORK))
		return (RET_ERROR);
	if (sem_wait(g_banquet.forks))
		return (RET_ERROR);
	if (print_status(philo, HAS_TAKEN_A_FORK))
		return (RET_ERROR);
	if (sem_post(g_banquet.ask_forks))
		return (RET_ERROR);
	if (sem_wait(philo->eating))
		return (RET_ERROR);
	philo->last_meal = get_time();
	philo->death_time = philo->last_meal + g_banquet.time_to_die;
	if (print_status(philo, IS_EATING))
		return (RET_ERROR);
	philo->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);
	if (sem_post(philo->eating))
		return (RET_ERROR);
	if (sem_post(philo->meat_count))
		return (RET_ERROR);
	return (RET_SUCCESS);
}
