/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:39:04 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/20 22:01:35 by francisberg      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
** Attention cette fonction est + complexe qu'elle en a l'air
**
** Pour rappel dans l'set_philos() on a lock le mutex philomutexeatcount
** Pour rappel, un philo ne mange jamais une deuxième fois quand un autre
** n'a pas encore mangé
** Sachant cela, on delock à chaque fois que le philo X mange et
** on le relock ici en incrémentant i
** On itère de manière croissant sur les philos donc si le philo 5 mange avant
** le 2, pas de problème car le mutex du 5 sera delock instantanément
** On fait cela "context.max_eat" fois
**
** void *arg ne sert à rien, c'est juste qu'on est forcé de mettre un argument
** par respect pour le prototype
*/

void		*handle_max_eat(void *arg)
{
	int nb_times_philos_have_eaten;
	int	i;

	(void)arg;
	nb_times_philos_have_eaten = 0;
	while (nb_times_philos_have_eaten < g_banquet.max_eat)
	{
		i = 0;
		while (i < g_banquet.nb_philos)
			if (sem_wait(g_banquet.philos[i++].meat_count))
				return ((void *)RET_ERROR);
		nb_times_philos_have_eaten++;
	}
	if (print_status(NULL, MAX_EAT_REACHED))
		return ((void *)RET_ERROR);
	if (sem_post(g_banquet.death))
		return ((void *)RET_ERROR);
	return ((void *)RET_SUCCESS);
}

/*
** La fonction handle_death() surveille qu'aucun philosophe ne meure
** Un philosophe meurt s'il n'est pas entrain de manger et que son temps de
** survie depuis le dernier est depassé
** Si c'est le cas, on unlock mutexdeath qui fait que le main se termine
** car autrement, ca ralentirai largement les actions
**
** On usleep(1000) (indispensable) pour pas toujours avoir le
** philo->philomutex de locké
** Pas besoin de savoir si le philo est en train de manger car de toute facon
** le mutex global est lock donc s'il mange, le thread handle_death pourra pas
** check si le philo meurt
*/

void		*handle_death(void *philo_voided)
{
	t_philo		*p;

	p = (t_philo*)philo_voided;
	while (1)
	{
		if (sem_wait(p->eating))
			return ((void *)RET_ERROR);
		if (p->death_time < get_time())
		{
			print_status(p, DIED);
			if (sem_post(p->eating))
				return ((void *)RET_ERROR);
			if (sem_post(g_banquet.death))
				return ((void *)RET_ERROR);
			return (RET_SUCCESS);
		}
		if (sem_post(p->eating))
			return ((void *)RET_ERROR);
		usleep(1000);
	}
	return ((void *)RET_SUCCESS);
}

/*
** no_eat_limit() intervient quand il n'y a pas de limite de repas
** philo->last_meal = get_time();
** Cette ligne sert juste d'initialisation avant le premier repas d'un philo
** On crée un thread  moniteur (detaché) du thread appartenant au philo X
** On essaie dans le while (1) de prendre les 2 fourchettes voisines
** 	- Si oui le philo mange, libère les 2 fourchettes, dors puis pense
*/

void		*philo_life(void *philo_uncasted)
{
	t_philo		*philo;
	pthread_t	subthread;

	philo = (t_philo*)philo_uncasted;
	g_banquet.start_time = get_time();
	philo->last_meal = get_time();
	philo->death_time = philo->last_meal + g_banquet.time_to_die;
	if (pthread_create(&subthread, NULL, &handle_death, philo))
		return ((void *)RET_ERROR);
	pthread_detach(subthread);
	while (1)
		if (eat_sleep_think(philo))
			return ((void *)RET_ERROR);
	return ((void *)RET_SUCCESS);
}

/*
** On crée un thread détaché pour chaque philosophe
** On usleep(100) pour éviter qu'au début ils prennent tous leurs baguettes
** en même temps et génèrent un deadlock
*/

int		start_banquet(void)
{
	int			i;
	void		*philo;
	pthread_t	thread;

	i = -1;
	if (g_banquet.max_eat)
	{
		if (pthread_create(&thread, NULL, &handle_max_eat, NULL))
			return (RET_ERROR);
		pthread_detach(thread);
	}
	while (++i < g_banquet.nb_philos)
	{
		philo = (void*)(&g_banquet.philos[i]);
		if (pthread_create(&thread, NULL, &philo_life, philo))
			return (RET_ERROR);
		pthread_detach(thread);
		usleep(100);
	}
	return (RET_SUCCESS);
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
	if ((ac < 5 || ac > 6))
		return (ft_printerror("Wrong number of arguments\n", 0));
	if (parse_banquet_config(ac, av))
		return (ft_printerror("Argument out of range\n", 1));
	if (start_banquet())
		return (ft_printerror("Thread error\n", 1));
	sem_wait(g_banquet.death);
	ft_clean();
	return (RET_SUCCESS);
}
