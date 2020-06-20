/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 14:05:32 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/20 15:09:03 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	add_str_to_log(char *log, int *i, char *str)
{
	int j;

	j = -1;
	while (str[++j])
		log[(*i)++] = str[j];
}

void			add_nb_to_log(char *log, int *index, uint64_t n)
{
	uint64_t len;

	len = 1;
	while (n / len >= 10)
		len *= 10;
	while (len > 0)
	{
		log[(*index)++] = '0' + n / len % 10;
		len /= 10;
	}
}

void            add_status_to_log(char *log, int *i, const int status)
{
        if (status == HAS_TAKEN_A_FORK)
            add_str_to_log(log, i, "has taken a fork\n");
        else if (status == HAS_LEFT_ITS_FORKS)
            add_str_to_log(log, i, "has left its forks\n");
        else if (status == IS_EATING)
            add_str_to_log(log, i, "is eating\n");
        else if (status == IS_SLEEPING)
            add_str_to_log(log, i, "is sleeping\n");
        else if (status == IS_THINKING)
            add_str_to_log(log, i, "is thinking\n");
        else if (status == DIED)
            add_str_to_log(log, i, "died\n");
}

void			print_status(t_philo *philo, const int status)
{
	char	    log[50] = {0};
    int         i;
	static int	off;

	if (off == 0)
	{
    	i = 0;
		add_nb_to_log(log, &i, get_time() - g_banquet.start_time);
        add_str_to_log(log, &i, "\t");
		if (status == MAX_EAT_REACHED)
		{
			off = 1;
            add_str_to_log(log, &i, "max eat reached\n");
			pthread_mutex_lock(&g_banquet.write);
            write(1, log, i);
			pthread_mutex_unlock(&g_banquet.write);
			return ;
		}
		add_nb_to_log(log, &i, philo->pos + 1);
        add_str_to_log(log, &i, " ");
        add_status_to_log(log, &i, status);
        if (status == DIED)
            off = 1;
		pthread_mutex_lock(&g_banquet.write);
        write(1, log, i);
	}
	pthread_mutex_unlock(&g_banquet.write);
}