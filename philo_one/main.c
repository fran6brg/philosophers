/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:00 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:43:30 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			*handle_max_eat(void *arg)
{
	int			nb_times_philos_have_eaten;
	int			i;

	(void)arg;
	nb_times_philos_have_eaten = 0;
	while (nb_times_philos_have_eaten < g_banquet.max_eat)
	{
		i = 0;
		while (i < g_banquet.nb_philos)
			pthread_mutex_lock(&g_banquet.philos[i++].eat_counter);
		nb_times_philos_have_eaten++;
	}
	print_log(NULL, MAX_EAT_REACHED);
	pthread_mutex_unlock(&g_banquet.stop_banquet);
	return (0);
}

void			*handle_death(void *philo_voided)
{
	t_philo		*p;

	p = (t_philo*)philo_voided;
	while (1)
	{
		usleep(1000);
		pthread_mutex_lock(&p->eating);
		if (p->death_time < get_time())
		{
			print_log(p, DIED);
			pthread_mutex_unlock(&p->eating);
			pthread_mutex_unlock(&g_banquet.stop_banquet);
			return (0);
		}
		pthread_mutex_unlock(&p->eating);
	}
}

void			*philo_life(void *philo_voided)
{
	t_philo		*p;
	pthread_t	death;

	p = (t_philo*)philo_voided;
	g_banquet.start_time = get_time();
	p->last_meal = g_banquet.start_time;
	p->death_time = p->last_meal + g_banquet.time_to_die;
	if (pthread_create(&death, NULL, &handle_death, p))
		return ((void *)RET_ERROR);
	pthread_detach(death);
	while (1)
		eat_sleep_think(p);
	return ((void *)RET_SUCCESS);
}

int				start_banquet(void)
{
	int			i;
	pthread_t	t;

	if (g_banquet.max_eat)
	{
		if (pthread_create(&t, NULL, &handle_max_eat, NULL))
			return (RET_ERROR);
		pthread_detach(t);
	}
	i = 0;
	while (i < g_banquet.nb_philos)
	{
		if (pthread_create(&t, NULL, &philo_life,
		(void *)(&g_banquet.philos[i++])))
			return (RET_ERROR);
		pthread_detach(t);
		usleep(100);
	}
	return (RET_SUCCESS);
}

int				main(int argc, char *argv[])
{
	if ((argc < 5 || argc > 6))
		return (ft_printerror("Wrong number of arguments\n", 0));
	if (parse_banquet_config(argc, argv))
		return (ft_printerror("Argument out of range\n", 1));
	if (start_banquet())
		return (ft_printerror("Thread error\n", 1));
	pthread_mutex_lock(&g_banquet.stop_banquet);
	pthread_mutex_unlock(&g_banquet.stop_banquet);
	ft_clean();
	return (RET_SUCCESS);
}
