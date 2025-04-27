/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:07:11 by oeddamou          #+#    #+#             */
/*   Updated: 2025/04/26 16:49:04 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_easy_init(int ac, char **av, t_data *d)
{
	memset(d, 0, sizeof(t_data));
	d->num_of_philos = ft_atoi(av[1]);
	d->time_to_die = ft_atoi(av[2]);
	d->time_to_eat = ft_atoi(av[3]);
	d->time_to_sleep = ft_atoi(av[4]);
	d->n_eat = -2;
	if (ac == 6)
		d->n_eat = ft_atoi(av[5]);
	d->dead_flag = 0;
	d->finish = 0;
	pthread_mutex_init(&d->dead_lock, NULL);
	pthread_mutex_init(&d->write_lock, NULL);
	pthread_mutex_init(&d->id_lock, NULL);
	pthread_mutex_init(&d->finish_lock, NULL);
	d->id = 0;
}

int	ft_stock_arg(int ac, char **av, t_data *d)
{
	int	i;

	ft_easy_init(ac, av, d);
	if (d->num_of_philos < 1 || d->time_to_die < 1 || d->time_to_eat < 1
		|| d->time_to_sleep < 1 || d->n_eat == -1)
		return (0);
	d->philos = malloc(sizeof(t_philo) * d->num_of_philos);
	if (!d->philos)
		return (0);
	d->forks = malloc(sizeof(pthread_mutex_t) * d->num_of_philos);
	if (!d->forks)
		return (free(d->philos), 0);
	i = -1;
	while (++i < d->num_of_philos)
	{
		pthread_mutex_init(&d->forks[i], NULL);
		d->philos[i].l_fork = &d->forks[i];
		if (i != 0)
			d->philos[i].r_fork = &(d->forks[i - 1]);
	}
	d->philos[0].r_fork = &(d->forks[d->num_of_philos - 1]);
	return (1);
}

void	ft_dead_test(t_data *d)
{
	int	i;
	int	finish;

	i = -1;
	finish = 0;
	while (!ft_get(&d->dead_flag, &d->dead_lock) && finish != d->num_of_philos)
	{
		usleep(10);
		finish = ft_get(&d->finish, &d->finish_lock);
		i = ft_dead(d);
		if (i > -1)
		{
			pthread_mutex_lock(&d->write_lock);
			printf("%ld %d died\n", ft_time() - d->start_time, i + 1);
			pthread_mutex_unlock(&d->write_lock);
			break ;
		}
	}
}

int	ft_inisial(t_data *d)
{
	int	i;

	i = -1;
	d->start_time = ft_time();
	while (++i < d->num_of_philos)
	{
		d->philos[i].meals_eaten = 0;
		d->philos[i].last_meal = d->start_time;
		pthread_mutex_init(&d->philos[i].meal_lock, NULL);
		if (pthread_create(&(d->philos[i].thread), NULL, ft_routine, (void *)d))
		{
			while (--i >= 0)
				pthread_join(d->philos[i].thread, NULL);
			return (free(d->philos), -1);
		}
	}
	ft_dead_test(d);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;
	int		v;

	if (ac > 6 || ac < 5 || !ft_stock_arg(ac, av, &data))
	{
		write(2, "Error: argements not valid\n", 27);
		return (1);
	}
	i = -1;
	v = ft_inisial(&data);
	while (++i < data.num_of_philos)
	{
		if (v == 0)
			pthread_join(data.philos[i].thread, NULL);
		pthread_mutex_destroy(&data.philos[i].meal_lock);
		pthread_mutex_destroy(&data.forks[i]);
	}
	pthread_mutex_destroy(&data.id_lock);
	pthread_mutex_destroy(&data.dead_lock);
	pthread_mutex_destroy(&data.write_lock);
	pthread_mutex_destroy(&data.finish_lock);
	free(data.forks);
	free(data.philos);
	return (0);
}
