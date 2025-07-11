/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:35:12 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 17:36:58 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_dead(t_data *d)
{
	long	t_from_eat;
	int		first_dead;
	long	t_first_dead;
	int		i;

	i = 0;
	first_dead = -1;
	t_first_dead = -1;
	while (i < d->num_of_philos)
	{
		pthread_mutex_lock(&d->philos[i].meal_lock);
		t_from_eat = ft_time() - d->philos[i].last_meal;
		pthread_mutex_unlock(&d->philos[i].meal_lock);
		if (t_from_eat > d->time_to_die)
		{
			ft_set(&d->dead_flag, &d->dead_lock, 1, 'n');
			if (t_from_eat > t_first_dead)
			{
				t_first_dead = t_from_eat;
				first_dead = i;
			}
		}
		i++;
	}
	return (first_dead);
}

void	ft_print(t_data *d, int id, char type)
{
	long	date;
	int		dead;

	pthread_mutex_lock(&d->write_lock);
	date = ft_time() - d->start_time;
	dead = ft_get(&d->dead_flag, &d->dead_lock);
	if (type == 'f' && !dead)
		printf("%ld %d has taken a fork\n", date, id + 1);
	else if (type == 'e' && !dead)
		printf("%ld %d is eating\n", date, id + 1);
	else if (type == 's' && !dead)
		printf("%ld %d is sleeping\n", date, id + 1);
	else if (type == 't' && !dead)
		printf("%ld %d is thinking\n", date, id + 1);
	pthread_mutex_unlock(&d->write_lock);
}

void	ft_eat(t_data *d, int id)
{
	if (!ft_get(&d->dead_flag, &d->dead_lock))
	{
		pthread_mutex_lock(d->philos[id].l_fork);
		ft_print(d, id, 'f');
		pthread_mutex_lock(d->philos[id].r_fork);
		ft_print(d, id, 'f');
		ft_print(d, id, 'e');
		pthread_mutex_lock(&d->philos[id].meal_lock);
		d->philos[id].last_meal = ft_time();
		pthread_mutex_unlock(&d->philos[id].meal_lock);
		ft_usleep(d->time_to_eat, d);
		pthread_mutex_unlock(d->philos[id].r_fork);
		pthread_mutex_unlock(d->philos[id].l_fork);
		if (d->philos[id].meals_eaten != -2)
			d->philos[id].meals_eaten++;
		if (!(d->philos[id].meals_eaten != d->n_eat || d->n_eat < 0))
			ft_set(&d->finish, &d->finish_lock, 0, 'i');
		if (ft_get(&d->finish, &d->finish_lock) < d->num_of_philos)
		{
			ft_print(d, id, 's');
			ft_usleep(d->time_to_sleep, d);
		}
		if (ft_get(&d->finish, &d->finish_lock) < d->num_of_philos)
			ft_print(d, id, 't');
	}
}

void	*ft_routine(void *d)
{
	t_data	*p;
	int		f_l;

	p = (t_data *)d;
	f_l = ft_set(&p->id, &p->id_lock, 0, 'i');
	ft_print(p, f_l, 't');
	if (p->num_of_philos == 1)
	{
		pthread_mutex_lock(p->philos[f_l].l_fork);
		ft_print(p, f_l, 'f');
		ft_usleep(p->time_to_die + 1, d);
		pthread_mutex_unlock(p->philos[f_l].l_fork);
	}
	if (f_l % 2 == 0)
		usleep(p->time_to_eat / 2 * 1000);
	while (!ft_get(&p->dead_flag, &p->dead_lock) && ft_get(&p->finish,
			&p->finish_lock) < p->num_of_philos)
	{
		ft_eat(p, f_l);
	}
	return (NULL);
}
