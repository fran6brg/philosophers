/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: francisberger <francisberger@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 16:41:04 by francisberg       #+#    #+#             */
/*   Updated: 2020/06/20 00:57:41 by francisberg      ###   ########.fr       */
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

void			*handle_max_eat(void *arg)
{
	int			i;
	int			max;

	max = -1;
	(void)arg;
	while (++max < g_banquet.max_eat)
	{
		i = -1;
		while (++i < g_banquet.nb_philos)
			if (sem_wait(g_banquet.philos[i].philosemaeatcount))
				return ((void*)1);
	}
	if (print_status(NULL, "maximum meal reached"))
		return ((void*)1);
	if (sem_post(g_banquet.death))
		return ((void*)1);
	return ((void*)0);
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

void		*handle_death(void *philo_uncasted)
{
	t_philo		*philo;

	philo = (t_philo*)philo_uncasted;
	while (42)
	{
		if (sem_wait(philo->philosema))
			return ((void*)1);
		if (philo->remainingtime < chrono())
		{
			print_status(philo, "died");
			if (sem_post(philo->philosema))
				return ((void*)1);
			if (sem_post(g_banquet.death))
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

int		philo_life(void *philo_uncasted)
{
	t_philo		*philo;
	pthread_t	subthread;
	int			usecwait;

	philo = (t_philo*)philo_uncasted;
	usecwait = (philo->pos - g_banquet.nb_philos) * -100;
	usleep(usecwait);
	philo->last_meal = chrono();
	philo->remainingtime = philo->last_meal + g_banquet.time_to_die;
	if (pthread_create(&subthread, NULL, &handle_death, philo))
		return (RET_ERROR);
	pthread_detach(subthread);
	while (42)
	{
		if (eat(philo))
			return (RET_ERROR);
		if (sleep_think(philo))
			return (RET_ERROR);
	}
	return (RET_SUCCESS);
}

int		start_banquet(void)
{
	int			i;

	i = -1;
	g_banquet.start_time = chrono();
	while (++i < g_banquet.nb_philos)
	{
		g_banquet.philos[i].pid = fork();
		if (g_banquet.philos[i].pid < 0)
			return (RET_ERROR);
		else if (g_banquet.philos[i].pid == 0)
		{
			philo_life(&g_banquet.philos[i]);
			exit(0);
		}
	}
	return (RET_SUCCESS);
}

/*
** Explication globale philo_three
** On execute threadmax_eat() qui lance un thread en loop infinie pour gerer
** le eat maximum qui est définit par les semaphores
** (donc accessible depuis tous les thread et processus)
** suivant : philos[i].philosemaeatcount
**
** Ensuite on start_banquet() qui fork() pour chaque philo du quel on call philo_life()
** dans laquelle on essaie en loop infinie de manger etc ...
**
** Usefull commands : pkill -f <match processus name>
*/

int				main(int ac, char **av)
{
	int i;

	i = 0;
	if ((ac < 5 || ac > 6))
		return (ft_printerror("Wrong number of arguments\n", 0));
	if (parse_banquet_config(ac, av))
		return (ft_printerror("Argument out of range\n", 1));
	if (start_banquet() || threadmax_eat())
		return (ft_printerror("Thread error\n", 1));
	sem_wait(g_banquet.death);
	while (i < g_banquet.nb_philos)
		kill(g_banquet.philos[i++].pid, SIGKILL);
	ft_clean();
	return (RET_SUCCESS);
}
