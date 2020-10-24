/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:54 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:45:16 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void			get_name(char *name, int id, int eat)
{
	int			i;
	const char	prefix[6] = "philo-";
	const char	prefix_eat[10] = "philo-eat-";

	i = -1;
	if (eat)
		while (++i < 10)
			name[i] = prefix_eat[i];
	else
		while (++i < 6)
			name[i] = prefix[i];
	while (id)
	{
		name[i++] = (id % 10) + '0';
		id /= 10;
	}
	name[i] = '\0';
}

int				set_philos(void)
{
	int			i;
	char		name[50];

	i = 0;
	while (i < g_banquet.nb_philos)
	{
		g_banquet.philos[i].pos = i;
		g_banquet.philos[i].last_meal = 0;
		g_banquet.philos[i].meal_count = 0;
		get_name(name, i + 1, PHI_INIT);
		sem_unlink(name);
		if ((g_banquet.philos[i].eating =
			sem_open(name, O_CREAT, 0666, 1)) == SEM_FAILED)
			return (RET_ERROR);
		get_name(name, i + 1, EAT_INIT);
		sem_unlink(name);
		if ((g_banquet.philos[i].eat_count =
			sem_open(name, O_CREAT, 0666, 0)) == SEM_FAILED)
			return (RET_ERROR);
		i++;
	}
	return (RET_SUCCESS);
}

int				set_semas(void)
{
	if ((g_banquet.ask_forks =
		sem_open(ASKFORKS, O_CREAT, 0666, 1)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.forks =
		sem_open(FORKS, O_CREAT, 0666, g_banquet.nb_philos)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.write =
		sem_open(WRITE, O_CREAT, 0666, 1)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.death =
		sem_open(DEATH, O_CREAT, 0666, 0)) == SEM_FAILED)
		return (RET_ERROR);
	if ((g_banquet.off =
		sem_open(OFF, O_CREAT, 0666, 1)) == SEM_FAILED)
		return (RET_ERROR);
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
	if (set_philos() || set_semas())
		return (RET_ERROR);
	return (RET_SUCCESS);
}
