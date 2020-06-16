/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 23:29:51 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/16 17:03:26 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** ft_printerror
*/

int 			ft_printerror(char *msg, int clean)
{
	if (clean)
		ft_clean();
	ft_putstrfd(msg, 2);
	return (RET_ERROR);
}

/*
** ft_clean
*/

int				ft_clean(void)
{
	int			i;

	i = -1;
	if (g_banquet.forks)
		while (++i < g_banquet.nb_philos)
			pthread_mutex_destroy(&g_banquet.forks[i]);
	free(g_banquet.forks);
	i = -1;
	if (g_banquet.philos)
		while (++i < g_banquet.nb_philos)
		{
			pthread_mutex_destroy(&g_banquet.philos[i].eating);
			pthread_mutex_destroy(&g_banquet.philos[i].eat_counter);
		}
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	pthread_mutex_destroy(&g_banquet.stop_banquet);
	pthread_mutex_destroy(&g_banquet.write);
	return (RET_ERROR);
}