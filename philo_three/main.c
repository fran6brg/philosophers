/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:11:08 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:43:30 by francisberg      ###   ########.fr       */
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
			if (sem_wait(g_banquet.philos[i].eat_count))
				return ((void *)RET_ERROR);
	}
	if (print_log(NULL, MAX_EAT_REACHED))
		return ((void *)RET_ERROR);
	if (sem_post(g_banquet.death))
		return ((void *)RET_ERROR);
	return ((void*)RET_SUCCESS);
}

void			*handle_death(void *philo_voided)
{
	t_philo		*p;

	p = (t_philo*)philo_voided;
	while (1)
	{
		if (sem_wait(p->eating))
			return ((void*)RET_ERROR);
		if (p->death_time < get_time())
		{
			print_log(p, DIED);
			if (sem_post(p->eating))
				return ((void*)RET_ERROR);
			if (sem_post(g_banquet.death))
				return ((void*)RET_ERROR);
			return ((void*)RET_SUCCESS);
		}
		if (sem_post(p->eating))
			return ((void*)RET_ERROR);
		usleep(1000);
	}
	return ((void*)RET_SUCCESS);
}

int				philo_life(void *philo_voided)
{
	t_philo		*p;
	pthread_t	death;

	p = (t_philo*)philo_voided;
	usleep(100);
	p->last_meal = get_time();
	p->death_time = p->last_meal + g_banquet.time_to_die;
	if (pthread_create(&death, NULL, &handle_death, p))
		return (RET_ERROR);
	pthread_detach(death);
	while (1)
		if (eat_sleep_think(p))
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

int				main(int argc, char *argv[])
{
	int			i;

	i = 0;
	if ((argc < 5 || argc > 6))
		return (ft_printerror("Wrong number of arguments\n", 0));
	if (parse_banquet_config(argc, argv))
		return (ft_printerror("Argument out of range\n", 1));
	if (start_banquet())
		return (ft_printerror("Thread error\n", 1));
	sem_wait(g_banquet.death);
	while (i < g_banquet.nb_philos)
		kill(g_banquet.philos[i++].pid, SIGKILL);
	ft_clean();
	return (RET_SUCCESS);
}
