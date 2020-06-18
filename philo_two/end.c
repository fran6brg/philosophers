/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/16 20:07:48 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/18 17:01:54 by francisberg      ###   ########.fr       */
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
	sem_unlink(FORKS);
	sem_unlink(WRITE);
	sem_unlink(DEATH);
	sem_unlink(ASKTAKEFORKS);
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	return (RET_ERROR);
}
