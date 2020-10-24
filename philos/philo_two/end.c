/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   end.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/21 02:11:42 by user42            #+#    #+#             */
/*   Updated: 2020/06/22 18:15:46 by francisberg      ###   ########.fr       */
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
	char	sem_name[50];

	sem_unlink(ASKFORKS);
	sem_unlink(FORKS);
	sem_unlink(WRITE);
	sem_unlink(DEATH);
	i = 0;
	if (g_banquet.philos)
	{
		while (i < g_banquet.nb_philos)
		{
			get_name(sem_name, i + 1, PHI_INIT);
			sem_unlink(sem_name);
			get_name(sem_name, i + 1, EAT_INIT);
			sem_unlink(sem_name);
			i++;
		}
	}
	free(g_banquet.philos);
	g_banquet.philos = NULL;
	return (RET_ERROR);
}
