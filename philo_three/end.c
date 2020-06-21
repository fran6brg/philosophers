/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:47 by user42            #+#    #+#             */
/*   Updated: 2020/06/21 02:10:48 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** ft_printerror
*/

int			ft_printerror(char *msg, int clean)
{
	if (clean)
		ft_clean();
	ft_putstrfd(msg, 2);
	return (RET_ERROR);
}

/*
** ft_clean
*/

int			ft_clean(void)
{
	int		i;
	char	name[50];

	i = -1;
	sem_unlink(FORKS);
	sem_unlink(WRITE);
	sem_unlink(DEATH);
	sem_unlink(PROCESSDEATH);
	sem_unlink(ASKTAKEFORKS);
	if (g_banquet.philos)
		while (++i < g_banquet.nb_philos)
		{
			semanames(name, i + 1, PHI_INIT);
			sem_unlink(name);
			semanames(name, i + 1, EAT_INIT);
			sem_unlink(name);
		}
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	sem_close(g_banquet.forks);
	sem_close(g_banquet.death);
	sem_close(g_banquet.write);
	sem_close(g_banquet.process_death);
	sem_close(g_banquet.ask_forks);
	return (RET_ERROR);
}
