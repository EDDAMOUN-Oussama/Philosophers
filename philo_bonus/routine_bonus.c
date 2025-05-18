/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 20:12:42 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 15:39:10 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_kill(t_philo *p, int stop)
{
	int	i;
	int	s;

	i = -1;
	while (++i < stop)
		kill(p[i].pid, 9);
	i = -1;
	while (++i < stop)
		waitpid(-1, &s, 0);
}

void	ft_print(t_philo *p, char type)
{
	long	date;

	sem_wait(p->data->print_sem);
	date = ft_time() - p->data->start_time;
	if (type == 'f')
		printf("%ld %d has taken a fork\n", date, p->id);
	else if (type == 'e')
		printf("%ld %d is eating\n", date, p->id);
	else if (type == 's')
		printf("%ld %d is sleeping\n", date, p->id);
	else if (type == 't')
		printf("%ld %d is thinking\n", date, p->id);
	sem_post(p->data->print_sem);
}

void	*ft_test_dead(void *d)
{
	t_philo	*p;
	long	time_form_eat;

	p = (t_philo *)d;
	while (1)
	{
		time_form_eat = ft_time() - get_last_meal(p);
		if (time_form_eat >= p->data->t_to_die)
		{
			sem_wait(p->data->print_sem);
			printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
			sem_unlink(p->name_sem);
			exit(-1);
		}
		if (time_form_eat - p->data->t_to_die > 2)
			usleep((time_form_eat - p->data->t_to_die) * 800);
		else
			usleep(500);
	}
	return (NULL);
}

void	ft_handle_one(t_philo *p)
{
	sem_wait(p->data->forks_sem);
	ft_print(p, 'f');
	ft_usleep(p->data->t_to_die + 1, p);
	printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
	ft_clean(p, 0, -1, 1);
}

void	ft_routine(t_philo *p)
{
	p->last_meal = p->data->start_time;
	if (p->data->num_of_philos == 1)
		ft_handle_one(p);
	if (pthread_create(&p->thread, NULL, ft_test_dead, (void *)p))
		ft_clean(p, -1, 0, 1);
	while (1)
	{
		ft_print(p, 't');
		sem_wait(p->data->forks_sem);
		ft_print(p, 'f');
		sem_wait(p->data->forks_sem);
		ft_print(p, 'f');
		ft_print(p, 'e');
		sem_wait(p->last_meal_sem);
		p->last_meal = ft_time();
		sem_post(p->last_meal_sem);
		ft_usleep(p->data->t_to_eat, p);
		sem_post(p->data->forks_sem);
		sem_post(p->data->forks_sem);
		p->meals_eaten++;
		if (p->meals_eaten == p->data->n_eat && p->data->n_eat >= 0)
			sem_post(p->data->finish_sem);
		ft_print(p, 's');
		ft_usleep(p->data->t_to_sleep, p);
	}
}
