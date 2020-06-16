/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:40:34 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/16 19:36:33 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int					ft_strcomp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
}

/*
**	man gettimeofday
**	tv_sec = seconds since Jan. 1, 1970
**	tv_usec = microseconds left (need to divide by 1000 for miliseconds)
**  Avec #include <inttypes.h>
** 	printf("start_time : %" PRIu64 "\n", get_time());
*/

uint64_t			get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec) * (uint64_t)1000 + (tv.tv_usec / 1000));
}

void				ft_putstrfd(char *str, int fd)
{
	int i;

	i = -1;	
	while (str[++i])
		write(fd, &str[i], 1);
}

int					ft_atoi(char *str)
{
	int		i;
	int		sign;
	int		result;

	i = 0;
	sign = 1;
	while (str[i] != '\0' && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
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
