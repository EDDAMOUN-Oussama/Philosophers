/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:39:48 by oeddamou          #+#    #+#             */
/*   Updated: 2025/04/27 08:29:38 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_data_init(int ac, char **av, t_philo *p)
{
	memset(p, 0, sizeof(t_philo));
	p->num_of_philos = ft_atoi(av[1]);
	p->time_to_die = ft_atoi(av[2]);
	p->time_to_eat = ft_atoi(av[3]);
	p->time_to_sleep = ft_atoi(av[4]);
	p->n_eat = -2;
	if (ac == 6)
		p->n_eat = ft_atoi(av[5]);
	p->start_time = ft_time();
	if (p->num_of_philos < 1 || p->time_to_die < 1 || p->time_to_eat < 1
		|| p->time_to_sleep < 1 || p->n_eat == -1)
		return (0);
	p->forks_sem = sem_open("/forks", O_EXCL | O_CREAT, 0644, p->num_of_philos);
	p->print_sem = sem_open("/print", O_EXCL | O_CREAT, 0644, 1);
	p->last_meal_sem = sem_open("/last_meal", O_EXCL | O_CREAT, 0644, 1);
	return (1);
}

void	ft_print(t_philo *p, char type)
{
	long	date;
	int		dead;

	dead = 0;
	date = ft_time() - p->start_time;
	sem_wait(p->print_sem);
	if (type == 'f' && !dead)
		printf("%ld %d has taken a fork\n", date, p->id);
	else if (type == 'e' && !dead)
		printf("%ld %d is eating\n", date, p->id);
	else if (type == 's' && !dead)
		printf("%ld %d is sleeping\n", date, p->id);
	else if (type == 't' && !dead)
		printf("%ld %d is thinking\n", date, p->id);
	sem_post(p->print_sem);
}

void	*ft_test_dead(void *d)
{
	t_philo	*p;
	int		dead;
	long	last_meal;

	p = (t_philo *)d;
	dead = 0;
	while (!dead)
	{
		sem_wait(p->last_meal_sem);
		last_meal = p->last_meal;
		sem_post(p->last_meal_sem);
		if (ft_time() - last_meal >= p->time_to_die)
		{
			sem_wait(p->print_sem);
			printf("%ld %d died\n", ft_time() - p->start_time, p->id);
			// sem_post(p->print_sem);
			exit(-1);
		}
		usleep(10);
	}
	return (NULL);
}

void	ft_routine(t_philo *p)
{
	if (p->id % 2 == 0)
		ft_usleep(p->time_to_eat / 2);
	if (!pthread_create(&p->thread, NULL, ft_test_dead, (void *)p))
		while ((p->meals_eaten < p->n_eat || p->n_eat < 0) && p->last_meal
			- ft_time() < p->time_to_die)
		{
			sem_wait(p->forks_sem);
			ft_print(p, 'f');
			sem_wait(p->forks_sem);
			ft_print(p, 'f');
			ft_print(p, 'e');
			ft_usleep(p->time_to_eat);
			sem_wait(p->last_meal_sem);
			p->last_meal = ft_time();
			sem_post(p->last_meal_sem);
			sem_post(p->forks_sem);
			sem_post(p->forks_sem);
			p->meals_eaten++;
			if ((p->meals_eaten >= p->n_eat && p->n_eat >= 0))
			{
				pthread_join(p->thread, NULL);
				exit(0);
			}
			ft_print(p, 's');
			ft_usleep(p->time_to_sleep);
			ft_print(p, 't');
		}
	pthread_join(p->thread, NULL);
	exit(0);
}

void	ft_inisial(t_philo *p)
{
	int		i;
	int		status;
	pid_t	pid;

	// pid_t	*pid;
	i = -1;
	// p = malloc(sizeof(t_philo) * d->num_of_philos);
	// if (!p)
	// exit(1);
	p->pid = malloc(sizeof(pid_t) * p->num_of_philos);
	if (!p->pid)
	{
		// free(p);
		exit(1);
	}
	while (++i < p->num_of_philos)
	{
		p->id = i + 1;
		p->last_meal = p->start_time;
		p->meals_eaten = 0;
		// p[i].data = *d;
		p->pid[i] = fork();
		if (p->pid[i] == 0)
			ft_routine(p);
	}
	while (--i >= 0)
	{
		waitpid(-1, &status, 0);
		if (status != 0)
		{
			i = 0;
			while (i < p->num_of_philos)
			{
				kill(p->pid[i++], SIGKILL);
			}
			sem_post(p->print_sem);
			break ;
		}
	}
}

int	main(int ac, char **av)
{
	int		i;
	int		v;
	t_philo	philo;

	// t_data	data;
	if (ac > 6 || ac < 5 || !ft_data_init(ac, av, &philo))
	{
		write(2, "Error: argements not valid\n", 27);
		return (1);
	}
	ft_inisial(&philo);
	sem_close(philo.forks_sem);
	sem_close(philo.print_sem);
	sem_close(philo.last_meal_sem);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/last_meal");
	free(philo.pid);
}
