/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: henri <henri@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/26 12:07:04 by henri             #+#    #+#             */
/*   Updated: 2020/06/07 12:10:54 by henri            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Attention cette fonction est + complexe qu'elle en a l'air
**
** Pour rappel dans l'initphilos() on a lock le mutex philomutexeatcount
** Pour rappel, un philo ne mange jamais une deuxième fois quand un autre
** n'a pas encore mangé
** Sachant cela, on delock à chaque fois que le philo X mange et
** on le relock ici en incrémentant i
** On itère de manière croissant sur les philos donc si le philo 5 mange avant
** le 2, pas de problème car le mutex du 5 sera delock instantanément
** On fait cela "context.maxeat" fois
**
** void *arg ne sert à rien, c'est juste qu'on est forcé de mettre un argument
** par respect pour le prototype
*/

static void		*watchingmaxeat(void *arg)
{
	int			i;
	int			max;

	max = -1;
	(void)arg;
	while (++max < g_context.maxeat)
	{
		i = -1;
		while (++i < g_context.philosophers)
			if (sem_wait(g_context.philos[i].philosemaeatcount))
				return ((void*)1);
	}
	if (printstatus(NULL, "maximum meal reached"))
		return ((void*)1);
	if (sem_post(g_context.semadeath))
		return ((void*)1);
	return ((void*)0);
}

/*
** La fonction watching() surveille qu'aucun philosophe ne meure
** Un philosophe meurt s'il n'est pas entrain de manger et que son temps de
** survie depuis le dernier est depassé
** Si c'est le cas, on unlock mutexdeath qui fait que le main se termine
** car autrement, ca ralentirai largement les actions
**
** On usleep(1000) (indispensable) pour pas toujours avoir le
** philo->philomutex de locké
** Pas besoin de savoir si le philo est en train de manger car de toute facon
** le mutex global est lock donc s'il mange, le thread watching pourra pas
** check si le philo meurt
*/

static void		*watching(void *philo_uncasted)
{
	t_philo		*philo;

	philo = (t_philo*)philo_uncasted;
	while (42)
	{
		if (sem_wait(philo->philosema))
			return ((void*)1);
		if (philo->remainingtime < chrono())
		{
			printstatus(philo, "died");
			if (sem_post(philo->philosema))
				return ((void*)1);
			if (sem_post(g_context.semadeath))
				return ((void*)1);
			return ((void*)0);
		}
		if (sem_post(philo->philosema))
			return ((void*)1);
		usleep(1000);
	}
	return ((void*)0);
}

/*
** no_eat_limit() intervient quand il n'y a pas de limite de repas
** philo->last_meal = chrono();
** Cette ligne sert juste d'initialisation avant le premier repas d'un philo
** On crée un thread  moniteur (detaché) du thread appartenant au philo X
** On essaie dans le while (1) de prendre les 2 fourchettes voisines
** 	- Si oui le philo mange, libère les 2 fourchettes, dors puis pense
*/

static void		*noeatlimit(void *philo_uncasted)
{
	t_philo		*philo;
	pthread_t	subthread;

	philo = (t_philo*)philo_uncasted;
	g_context.timer = chrono();
	philo->last_meal = chrono();
	philo->remainingtime = philo->last_meal + g_context.time_to_die;
	if (pthread_create(&subthread, NULL, &watching, philo))
		return ((void*)1);
	pthread_detach(subthread);
	while (42)
	{
		if (eat(philo))
			return ((void*)1);
		if (sleep_unlock2forks(philo))
			return ((void*)1);
	}
	return ((void*)0);
}

/*
** On crée un thread détaché pour chaque philosophe
** On usleep(100) pour éviter qu'au début ils prennent tous leurs baguettes
** en même temps et génèrent un deadlock
*/

static int		start(void)
{
	int			i;
	void		*philo;
	pthread_t	thread;

	i = -1;
	if (g_context.maxeat)
	{
		if (pthread_create(&thread, NULL, &watchingmaxeat, NULL))
			return (1);
		pthread_detach(thread);
	}
	while (++i < g_context.philosophers)
	{
		philo = (void*)(&g_context.philos[i]);
		if (pthread_create(&thread, NULL, &noeatlimit, philo))
			return (1);
		pthread_detach(thread);
		usleep(100);
	}
	return (0);
}

/*
** On attend à pthread_mutex_lock(&context.mutexdeath) jusqu'à ce qu'un
** philosophe meurt
** On va wait la ligne pthread_mutex_lock(&context.mutexdeath);
** jusqu'à ce qu'un philosophe meurt car on a lock le mutex dans initmutex()
** auparavant
** Pour rappel, un "lock sur mutex déjà lock" = "wait jusqu'à unlock du mutex"
*/

int				main(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		putstrfd("Error: wrong number of arguments\n", 2);
		return (1);
	}
	if (initcontext(ac, av))
	{
		clear();
		putstrfd("Error: initialization\n", 2);
		return (1);
	}
	if (start())
	{
		clear();
		putstrfd("Error: core function\n", 2);
		return (1);
	}
	sem_wait(g_context.semadeath);
	clear();
	return (0);
}
