/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/20 00:05:27 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/20 00:07:00 by francisberg      ###   ########.fr       */
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
	char		name[50];

	i = -1;
	sem_unlink(SEMAFORKS);
	sem_unlink(write);
	sem_unlink(DEATH);
	sem_unlink(SEMAPROCESSDEATH);
	sem_unlink(ASKTAKEFORKS);
	if (g_banquet.philos)
	{
        while (++i < g_banquet.nb_philos)
		{
			semanames(name, i + 1, PHI_INIT);
			sem_unlink(name);
			semanames(name, i + 1, EAT_INIT);
			sem_unlink(name);
		}
    }
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	sem_close(g_banquet.semaforks);
	sem_close(g_banquet.death);
	sem_close(g_banquet.write);
	sem_close(g_banquet.semaprocessdeath);
	sem_close(g_banquet.semaskforks);
	return (RET_ERROR);
}
