/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:08:56 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 12:49:07 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_data_init(int ac, char **av, t_data *d)
{
	d->num_of_philos = ft_atoi(av[1]);
	d->t_to_die = ft_atoi(av[2]);
	d->t_to_eat = ft_atoi(av[3]);
	d->t_to_sleep = ft_atoi(av[4]);
	d->n_eat = -2;
	if (ac == 6)
		d->n_eat = ft_atoi(av[5]);
	if (d->num_of_philos < 1 || d->t_to_die < 1 || d->t_to_eat < 1
		|| d->t_to_sleep < 1 || d->n_eat == -1)
		return (0);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/finish");
	d->forks_sem = sem_open("/forks", O_EXCL | O_CREAT, 0644, d->num_of_philos);
	d->print_sem = sem_open("/print", O_EXCL | O_CREAT, 0644, 1);
	d->finish_sem = sem_open("/finish", O_EXCL | O_CREAT, 0644, 0);
	if (!d->forks_sem || !d->print_sem || !d->finish_sem)
	{
		sem_unlink("/forks");
		sem_unlink("/print");
		sem_unlink("/finish");
		printf("Error: open semaphore fatal\n");
		exit(1);
	}
	return (1);
}

int	ft_create_sem(t_philo *p, char *name)
{
	int	i;

	i = 0;
	while (name[i])
	{
		p->name_sem[i] = name[i];
		i++;
	}
	p->name_sem[i++] = ((int)(p->id / 100) + '0');
	p->name_sem[i++] = ((int)((p->id % 100) / 10) + '0');
	p->name_sem[i++] = ((int)((p->id % 100) % 10) + '0');
	p->name_sem[i++] = '\0';
	sem_unlink(p->name_sem);
	p->last_meal_sem = sem_open(p->name_sem, O_EXCL | O_CREAT, 0644, 1);
	if (!p->last_meal_sem)
		return (0);
	return (1);
}

void	ft_init(t_data *d, t_philo *p)
{
	int	i;

	i = -1;
	while (++i < d->num_of_philos)
	{
		memset(&p[i], 0, sizeof(t_philo));
		p[i].data = d;
		p[i].id = i + 1;
		p[i].meals_eaten = 0;
		if (!ft_create_sem(&p[i], "/meals_eat"))
		{
			printf("Error: open semaphore fatal\n");
			ft_clean(p, 1, i, 0);
		}
	}
}

void	ft_inisial(t_data *d, t_philo *p)
{
	int	i;

	ft_init(d, p);
	i = -1;
	d->start_time = ft_time();
	while (++i < d->num_of_philos)
	{
		p[i].pid = fork();
		if (p[i].pid < 0)
		{
			ft_kill(p, i);
			printf("Error in fork\n");
			ft_clean(p, 1, d->num_of_philos, 0);
		}
		if (p[i].pid == 0)
			ft_routine(&p[i]);
	}
	if (d->num_of_philos > 1 && d->n_eat > 0)
		if (pthread_create(&d->thread, NULL, ft_wait_finish, p))
			ft_clean(p, 1, d->num_of_philos, 0);
	ft_wait_kill(p, d);
}

int	main(int ac, char **av)
{
	t_data	data;
	t_philo	*p;

	memset(&data, 0, sizeof(t_data));
	if (ac > 6 || ac < 5 || !ft_data_init(ac, av, &data))
	{
		write(2, "Error: argements not valid\n", 27);
		return (1);
	}
	if (data.n_eat == 0)
		return (0);
	p = malloc(sizeof(t_philo) * data.num_of_philos);
	if (!p)
	{
		printf("Error: malloc falid\n");
		exit(1);
	}
	ft_inisial(&data, p);
	ft_clean(p, 0, data.num_of_philos, 0);
	return (0);
}
