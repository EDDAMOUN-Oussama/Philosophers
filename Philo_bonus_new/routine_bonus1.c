/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:38:32 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/10 19:30:45 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_test_dead(void *d)
{
	t_philo	*p;
	
	p = (t_philo *)d;
	while (1)
	{
		if (ft_time() - get_last_meal(p) > p->data->t_to_die)
		{
			sem_wait(p->data->print_sem);
			printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
	        sem_unlink(p->name_sem);
			exit(-1);
			// ft_clean(p, 0, -1, 1);//////////////////
		}
		usleep(900);
	}
	return (NULL);
}

void ft_handle_one(t_philo *p)
{
    sem_wait(p->data->forks_sem);
	ft_print(p, 'f');
    ft_usleep(p->data->t_to_die + 1, p);
	printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
	// sem_wait(p->data->print_sem);
	// sem_close(p->data->forks_sem);
	// sem_close(p->data->print_sem);
	// sem_close(p->last_meal_sem);
	// exit(-1);
	ft_clean(p, 0, -1, 1);
	
}

void ft_routine(t_philo *p)
{
	p->last_meal = p->data->start_time;
    if (p->data->num_of_philos == 1)
		ft_handle_one(p);
	if (!pthread_create(&p->thread, NULL, ft_test_dead, (void *)p))
    while (1)
    {
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
        if (p->meals_eaten >= p->data->n_eat && p->data->n_eat >= 0)
        {
            // sem_close(p->last_meal_sem);
            // sem_close(p->data->forks_sem);
	        // sem_close(p->data->print_sem);
	        // sem_unlink(p->name_sem);
            // exit(0);
			ft_clean(p, 0, 0, 1);
        }
        ft_print(p, 's');
		ft_usleep(p->data->t_to_sleep, p);
		ft_print(p, 't');
    }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
}
