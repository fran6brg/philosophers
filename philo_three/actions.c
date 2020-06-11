/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henri <henri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/28 15:38:15 by henri             #+#    #+#             */
/*   Updated: 2020/06/07 17:50:03 by henri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** threadmaxeat() est call en début de main pour lancer le thread en charge
** du check du semaphore gérant le maximum de eat
*/

int				threadmaxeat(void)
{
	pthread_t	thread;

	if (g_context.maxeat)
	{
		if (pthread_create(&thread, NULL, &watchingmaxeat, NULL))
			return (1);
		pthread_detach(thread);
	}
	return (0);
}

/*
** Dans philo_three, printstatus() fonctionne un peu différement :
** on utilise un sema binaire (donc mutex) en plus pour gérer l'affichage
** correctement entre les processus
** S'il y a un mort ou que l'on atteint le maximum de repas, on n'execute pas
** sem_post(context.semaprocessdeath) donc le semaphore (initialisé à 1), reste
** à 0 après ce qui bloque l'écriture
** Avant le static qui permettait de faire ça n'aurait pas fonctionné pour
** des multiples processus (contrairement à juste du multithread)
*/

int				printstatus(t_philo *philo, char *str)
{
	if (sem_wait(g_context.semawrite))
		return (1);
	if (sem_wait(g_context.semaprocessdeath))
		return (1);
	putuint64_t(1, chrono() - g_context.timer);
	write(1, "\t", 1);
	if (strcompare(str, "maximum meal reached") != 0)
		putuint64_t(1, ((uint64_t)philo->pos + 1));
	write(1, " ", 1);
	putstrfd(str, 1);
	write(1, "\n", 1);
	if (strcompare(str, "maximum meal reached") != 0 &&
		strcompare(str, "died") != 0)
		if (sem_post(g_context.semaprocessdeath))
			return (1);
	if (sem_post(g_context.semawrite))
		return (1);
	return (0);
}

static void		ft_usleep(unsigned int n)
{
	uint64_t	start;

	start = chrono();
	while (1)
	{
		usleep(50);
		if (chrono() - start >= n)
			break ;
	}
}

int				sleep_unlock2forks(t_philo *philo)
{
	if (printstatus(philo, "is sleeping"))
		return (1);
	if (sem_post(g_context.semaforks))
		return (1);
	if (sem_post(g_context.semaforks))
		return (1);
	ft_usleep(g_context.time_to_sleep);
	if (printstatus(philo, "is thinking"))
		return (1);
	if ((philo->pos + 1) % 2 == 0)
		usleep(500);
	return (0);
}

/*
** On lock le mutex appartenant au philosophe concerné
** usleep * 1000 pour les milisecondes
*/

int				eat(t_philo *philo)
{
	if (sem_wait(g_context.semaskforks))
		return (1);
	if (sem_wait(g_context.semaforks))
		return (1);
	if (printstatus(philo, "has taken a fork"))
		return (1);
	if (sem_wait(g_context.semaforks))
		return (1);
	if (printstatus(philo, "has taken a fork"))
		return (1);
	if (sem_post(g_context.semaskforks))
		return (1);
	if (sem_wait(philo->philosema))
		return (1);
	philo->last_meal = chrono();
	philo->remainingtime = philo->last_meal + g_context.time_to_die;
	if (printstatus(philo, "is eating"))
		return (1);
	philo->meal_count += 1;
	ft_usleep(g_context.time_to_eat);
	if (sem_post(philo->philosema))
		return (1);
	if (sem_post(philo->philosemaeatcount))
		return (1);
	return (0);
}
