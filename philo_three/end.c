/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:10:47 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:45:16 by francisberg      ###   ########.fr       */
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
	char	sema_name[50];

	sem_unlink(ASKFORKS);
	sem_unlink(FORKS);
	sem_unlink(WRITE);
	sem_unlink(OFF);
	sem_unlink(DEATH);
	i = 0;
	if (g_banquet.philos)
		while (i < g_banquet.nb_philos)
		{
			get_name(sema_name, i + 1, PHI_INIT);
			sem_unlink(sema_name);
			get_name(sema_name, i + 1, EAT_INIT);
			sem_unlink(sema_name);
			i++;
		}
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	sem_close(g_banquet.forks);
	sem_close(g_banquet.death);
	sem_close(g_banquet.write);
	sem_close(g_banquet.off);
	sem_close(g_banquet.ask_forks);
	return (RET_ERROR);
}
