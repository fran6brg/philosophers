/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:41:04 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/21 01:27:04 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			*handle_max_eat(void *arg)
{
	int			i;
	int			max;

	max = -1;
	(void)arg;
	while (++max < g_banquet.max_eat)
	{
		i = -1;
		while (++i < g_banquet.nb_philos)
			if (sem_wait(g_banquet.philos[i].philo_eat_count))
				return ((void *)RET_ERROR);
	}
	if (print_status(NULL, MAX_EAT_REACHED))
		return ((void *)RET_ERROR);
	if (sem_post(g_banquet.death))
		return ((void *)RET_ERROR);
	return ((void*)RET_SUCCESS);
}

void			*handle_death(void *philo_voided)
{
	t_philo		*philo;

	philo = (t_philo*)philo_voided;
	while (1)
	{
		if (sem_wait(philo->philosema))
			return ((void*)RET_ERROR);
		if (philo->remainingtime < get_time())
		{
			print_status(philo, DIED);
			if (sem_post(philo->philosema))
				return ((void*)RET_ERROR);
			if (sem_post(g_banquet.death))
				return ((void*)RET_ERROR);
			return ((void*)RET_SUCCESS);
		}
		if (sem_post(philo->philosema))
			return ((void*)RET_ERROR);
		usleep(1000);
	}
	return ((void*)RET_SUCCESS);
}

int				philo_life(void *philo_voided)
{
	t_philo		*philo;
	pthread_t	death;

	philo = (t_philo*)philo_voided;
	usleep(100);
	philo->last_meal = get_time();
	philo->remainingtime = philo->last_meal + g_banquet.time_to_die;
	if (pthread_create(&death, NULL, &handle_death, philo))
		return (RET_ERROR);
	pthread_detach(death);
	while (1)
		if (eat_sleep_think(philo))
			return (RET_ERROR);
	return (RET_SUCCESS);
}

int				start_banquet(void)
{
	int			i;
	pthread_t	max;

	i = -1;
	g_banquet.start_time = get_time();
	if (g_banquet.max_eat)
	{
		if (pthread_create(&max, NULL, &handle_max_eat, NULL))
			return (RET_ERROR);
		pthread_detach(max);
	}
	while (++i < g_banquet.nb_philos)
	{
		g_banquet.philos[i].pid = fork();
		if (g_banquet.philos[i].pid < 0)
			return (RET_ERROR);
		else if (g_banquet.philos[i].pid == 0)
		{
			philo_life(&g_banquet.philos[i]);
			exit(0);
		}
	}
	return (RET_SUCCESS);
}

int				main(int ac, char **av)
{
	int			i;

	i = 0;
	if ((ac < 5 || ac > 6))
		return (ft_printerror("Wrong number of arguments\n", 0));
	if (parse_banquet_config(ac, av))
		return (ft_printerror("Argument out of range\n", 1));
	if (start_banquet())
		return (ft_printerror("Thread error\n", 1));
	sem_wait(g_banquet.death);
	while (i < g_banquet.nb_philos)
		kill(g_banquet.philos[i++].pid, SIGKILL);
	ft_clean();
	return (RET_SUCCESS);
}
