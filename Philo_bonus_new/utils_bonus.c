/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:00:16 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/10 20:14:30 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi(char *str)
{
	int				i;
	unsigned long	r;

	i = 0;
	r = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
		i++;
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		r = r * 10 + (str[i] - '0');
		i++;
		if (r > 2147483647)
			return (-1);
	}
	if (i != 0 && !((str[i - 1] >= 9 && str[i - 1] <= 13) 
			|| str[i - 1] == ' '))
		while ((str[i] >= 9 && str[i] <= 13) || str[i] == ' ')
			i++;
	if (str[i] != '\0' || i == 0)
		return (-1);
	return ((int)r);
}

long	ft_time(void)
{
	struct timeval	time;

	if (!gettimeofday(&time, NULL))
		return (time.tv_sec * 1000 + time.tv_usec / 1000);
	else
		exit(2);
	return (-1);
}

int	ft_usleep(long milliseconds, t_philo *p)
{
	long	start;

	start = ft_time();
	while ((ft_time() - start) < milliseconds)
	{
		usleep(900);
		if (ft_time() - get_last_meal(p) > p->data->t_to_die)
		{
			sem_wait(p->data->print_sem);
			printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
			exit(-1);
		}
	}
	return (0);
}

long	get_last_meal(t_philo *p)
{
	long	last_meal;

	sem_wait(p->last_meal_sem);
	last_meal = p->last_meal;
	sem_post(p->last_meal_sem);
	return (last_meal);
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
		sem_close(p[0].data->forks_sem);
		sem_close(p[0].data->print_sem);
		sem_unlink("/forks");
		sem_unlink("/print");
		free(p);
	}
	else
	{
		sem_close(p->data->forks_sem);
		sem_close(p->data->print_sem);
		sem_close(p->last_meal_sem);
		sem_unlink(p->name_sem);
	}
	exit(ext);
}
