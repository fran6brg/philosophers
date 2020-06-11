/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henri <henri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/27 16:43:10 by henri             #+#    #+#             */
/*   Updated: 2020/06/07 12:04:12 by henri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			clear(void)
{
	int			i;

	i = -1;
	if (g_context.mutexforks)
		while (++i < g_context.philosophers)
			pthread_mutex_destroy(&g_context.mutexforks[i]);
	free(g_context.mutexforks);
	i = -1;
	if (g_context.philos)
		while (++i < g_context.philosophers)
		{
			pthread_mutex_destroy(&g_context.philos[i].philomutex);
			pthread_mutex_destroy(&g_context.philos[i].philomutexeatcount);
		}
	free(g_context.philos);
	g_context.philos = NULL;
	pthread_mutex_destroy(&g_context.mutexdeath);
	pthread_mutex_destroy(&g_context.mutexwrite);
}

/*
** Le table est circulaire donc le dernier philosophe est voisin du premier
** D'où le ternaire (i + 1 != number) ? i + 1 : 0
*/

static void		initphilos(int number)
{
	int			i;

	i = 0;
	while (i < number)
	{
		g_context.philos[i].pos = i;
		g_context.philos[i].last_meal = 0;
		g_context.philos[i].meal_count = 0;
		g_context.philos[i].lfork = i;
		g_context.philos[i].rfork = (i + 1 != number) ? i + 1 : 0;
		pthread_mutex_init(&g_context.philos[i].philomutex, NULL);
		pthread_mutex_init(&g_context.philos[i].philomutexeatcount, NULL);
		pthread_mutex_lock(&g_context.philos[i].philomutexeatcount);
		i++;
	}
}

/*
** On lock mutexdeath car c'est la condition de sortie du main
** Le pthread_mutex_lock(mutexdeath) ici permet de wait jusqu'à une mort
** dans le 2nd call de pthread_mutex_lock(mutexdeath) du main
*/

static int		initmutex(int number)
{
	int			i;

	g_context.mutexforks = NULL;
	if ((g_context.mutexforks = malloc(sizeof(pthread_mutex_t) * number)) == 0)
		return (1);
	i = -1;
	while (++i < number)
		pthread_mutex_init(&g_context.mutexforks[i], NULL);
	pthread_mutex_init(&g_context.mutexdeath, NULL);
	pthread_mutex_lock(&g_context.mutexdeath);
	pthread_mutex_init(&g_context.mutexwrite, NULL);
	return (0);
}

int				initcontext(int ac, char **av)
{
	clear();
	memset(&g_context, 0, sizeof(g_context));
	g_context.philosophers = ft_atoi(av[1]);
	g_context.time_to_die = ft_atoi(av[2]);
	g_context.time_to_eat = ft_atoi(av[3]);
	g_context.time_to_sleep = ft_atoi(av[4]);
	g_context.maxeat = (ac == 6) ? ft_atoi(av[5]) : 0;
	if (g_context.philosophers < 2 || g_context.philosophers > 200 ||
		g_context.time_to_die < 60 || g_context.time_to_eat < 60 ||
		g_context.time_to_sleep < 60 || g_context.maxeat < 0)
		return (1);
	g_context.philos = NULL;
	if (!(g_context.philos = malloc(sizeof(t_philo) * g_context.philosophers)))
		return (1);
	initphilos(g_context.philosophers);
	return (initmutex(g_context.philosophers));
}
