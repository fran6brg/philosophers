/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:39:09 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/18 16:12:05 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int					strcompare(char *s1, char *s2)
{
	int				i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

void				putuint64_t(int fd, uint64_t nbr)
{
	int				i;
	char			tmp[15];

	if (nbr == 0)
		tmp[0] = '0';
	i = 0;
	while (nbr != 0)
	{
		tmp[i++] = '0' + (nbr % 10);
		nbr /= 10;
	}
	i = (i > 0) ? i - 1 : i;
	while (i >= 0)
		write(fd, &tmp[i--], 1);
}

void				ft_putstrfd(char *str, int fd)
{
	while (*str)
	{
		write(fd, str, 1);
		str++;
	}
}

int					ft_atoi(char *str)
{
	int				i;
	int				sign;
	int				result;

	i = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == '\n' || str[i] == '\v'
			|| str[i] == '\f' || str[i] == '\r' || str[i] == ' ')
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	result = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}
