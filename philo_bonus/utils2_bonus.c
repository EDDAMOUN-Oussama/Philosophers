/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 12:45:57 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 12:54:15 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ft_wait_finish(void *data)
{
	int		i;
	t_philo	*p;

	p = (t_philo *)data;
	i = -1;
	while (++i < p->data->num_of_philos - 1)
		sem_wait(p->data->finish_sem);
	ft_kill(p, p->data->num_of_philos);
	return (NULL);
}

void	ft_wait_kill(t_philo *p, t_data *d)
{
	int	s;
	int	i;

	i = -1;
	while (++i < d->num_of_philos)
	{
		waitpid(-1, &s, 0);
		if (s != 0)
		{
			ft_kill(p, d->num_of_philos);
			i = -1;
			while (++i < d->num_of_philos)
				sem_post(d->finish_sem);
			pthread_join(d->thread, NULL);
		}
	}
}

void	ft_close(t_philo *p)
{
	sem_close(p->data->forks_sem);
	sem_close(p->data->print_sem);
	sem_close(p->data->finish_sem);
}

void	ft_clean(t_philo *p, int ext, int nb, int type)
{
	int	i;

	if (type == 0)
	{
		i = -1;
		while (++i < nb)
		{
			sem_close(p[i].last_meal_sem);
			sem_unlink(p[i].name_sem);
		}
		ft_close(&p[0]);
		sem_unlink("/forks");
		sem_unlink("/print");
		sem_unlink("/finish");
		free(p);
	}
	else
	{
		ft_close(p);
		sem_close(p->last_meal_sem);
		sem_unlink(p->name_sem);
	}
	exit(ext);
}
