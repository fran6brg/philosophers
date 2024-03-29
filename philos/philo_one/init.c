/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:09:44 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:39:11 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int				set_philos(void)
{
	int			i;

	i = 0;
	while (i < g_banquet.nb_philos)
	{
		g_banquet.philos[i].pos = i;
		g_banquet.philos[i].meal_count = 0;
		g_banquet.philos[i].last_meal = 0;
		pthread_mutex_init(&g_banquet.philos[i].eating, NULL);
		pthread_mutex_init(&g_banquet.philos[i].eat_counter, NULL);
		pthread_mutex_lock(&g_banquet.philos[i].eat_counter);
		g_banquet.philos[i].lfork = i;
		g_banquet.philos[i].rfork = (i + 1 != g_banquet.nb_philos) ? i + 1 : 0;
		i++;
	}
	return (RET_SUCCESS);
}

int				set_mutex(void)
{
	int			i;

	g_banquet.forks = NULL;
	if (!(g_banquet.forks = malloc(sizeof(pthread_mutex_t)
	* g_banquet.nb_philos)))
		return (RET_ERROR);
	i = 0;
	while (i < g_banquet.nb_philos)
		pthread_mutex_init(&g_banquet.forks[i++], NULL);
	pthread_mutex_init(&g_banquet.stop_banquet, NULL);
	pthread_mutex_lock(&g_banquet.stop_banquet);
	pthread_mutex_init(&g_banquet.write, NULL);
	return (RET_SUCCESS);
}

int				check_config(void)
{
	if (g_banquet.nb_philos <= 0
		|| g_banquet.time_to_die <= 0
		|| g_banquet.time_to_eat <= 0
		|| g_banquet.time_to_sleep <= 0
		|| g_banquet.max_eat < 0)
		return (RET_ERROR);
	return (RET_SUCCESS);
}

int				parse_banquet_config(int argc, char *argv[])
{
	ft_clean();
	memset(&g_banquet, 0, sizeof(g_banquet));
	g_banquet.nb_philos = ft_atoi(argv[1]);
	g_banquet.time_to_die = ft_atoi(argv[2]);
	g_banquet.time_to_eat = ft_atoi(argv[3]);
	g_banquet.time_to_sleep = ft_atoi(argv[4]);
	g_banquet.max_eat = (argc == 6) ? ft_atoi(argv[5]) : 0;
	if (check_config())
		return (RET_ERROR);
	g_banquet.philos = NULL;
	if (!(g_banquet.philos = malloc(sizeof(t_philo) * g_banquet.nb_philos)))
		return (RET_ERROR);
	if (set_philos() || set_mutex())
		return (RET_ERROR);
	return (RET_SUCCESS);
}
