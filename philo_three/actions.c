/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:40:43 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/20 00:50:49 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** threadmax_eat() est call en début de main pour lancer le thread en charge
** du check du semaphore gérant le maximum de eat
*/

int				threadmax_eat(void)
{
	pthread_t	thread;

	if (g_banquet.max_eat)
	{
		if (pthread_create(&thread, NULL, &handle_max_eat, NULL))
			return (RET_ERROR);
		pthread_detach(thread);
	}
	return (RET_SUCCESS);
}

static void		ft_loop_usleep(unsigned int n)
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

int				sleep_think(t_philo *philo)
{
	if (print_status(philo, "is sleeping"))
		return (RET_ERROR);
	if (sem_post(g_banquet.semaforks))
		return (RET_ERROR);
	if (sem_post(g_banquet.semaforks))
		return (RET_ERROR);
	ft_loop_usleep(g_banquet.time_to_sleep);
	if (print_status(philo, "is thinking"))
		return (RET_ERROR);
	return (RET_SUCCESS);
}

/*
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

int				eat(t_philo *philo)
{
	if (sem_wait(g_banquet.semaskforks))
		return (RET_ERROR);
	if (sem_wait(g_banquet.semaforks))
		return (RET_ERROR);
	if (print_status(philo, "has taken a fork"))
		return (RET_ERROR);
	if (sem_wait(g_banquet.semaforks))
		return (RET_ERROR);
	if (print_status(philo, "has taken a fork"))
		return (RET_ERROR);
	if (sem_post(g_banquet.semaskforks))
		return (RET_ERROR);
	if (sem_wait(philo->philosema))
		return (RET_ERROR);
	philo->last_meal = chrono();
	philo->remainingtime = philo->last_meal + g_banquet.time_to_die;
	if (print_status(philo, "is eating"))
		return (RET_ERROR);
	philo->meal_count += 1;
	ft_loop_usleep(g_banquet.time_to_eat);
	if (sem_post(philo->philosema))
		return (RET_ERROR);
	if (sem_post(philo->philosemaeatcount))
		return (RET_ERROR);
	return (RET_SUCCESS);
}
