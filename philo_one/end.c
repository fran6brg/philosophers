/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:09:34 by user42            #+#    #+#             */
/*   Updated: 2020/06/21 02:09:36 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** ft_printerror
*/

int		ft_printerror(char *msg, int clean)
{
	if (clean)
		ft_clean();
	ft_putstrfd(msg, 2);
	return (RET_ERROR);
}

/*
** ft_clean
*/

int		ft_clean(void)
{
	int i;

	i = 0;
	if (g_banquet.philos)
		while (i < g_banquet.nb_philos)
		{
			pthread_mutex_destroy(&g_banquet.philos[i].eating);
			pthread_mutex_destroy(&g_banquet.philos[i++].eat_counter);
		}
	free(g_banquet.philos);
	i = 0;
	if (g_banquet.forks)
		while (i < g_banquet.nb_philos)
			pthread_mutex_destroy(&g_banquet.forks[i++]);
	free(g_banquet.forks);
	g_banquet.philos = NULL;
	pthread_mutex_destroy(&g_banquet.stop_banquet);
	pthread_mutex_destroy(&g_banquet.write);
	return (RET_ERROR);
}
