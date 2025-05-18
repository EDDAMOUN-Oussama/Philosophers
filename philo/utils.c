/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:07:01 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 13:21:26 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(long milliseconds, t_data *d)
{
	long	start;

	start = ft_time();
	while ((ft_time() - start) < milliseconds)
	{
		if (ft_get(&d->dead_flag, &d->dead_lock))
			return (0);
		usleep(100);
	}
	return (0);
}

int	ft_get(int *value, pthread_mutex_t *lock)
{
	int	result;

	pthread_mutex_lock(lock);
	result = *value;
	pthread_mutex_unlock(lock);
	return (result);
}

int	ft_set(int *value, pthread_mutex_t *lock, int new_val, char type)
{
	int	old;

	if (type == 'i')
	{
		pthread_mutex_lock(lock);
		old = *value;
		(*value)++;
		pthread_mutex_unlock(lock);
	}
	else
	{
		pthread_mutex_lock(lock);
		old = *value;
		*value = new_val;
		pthread_mutex_unlock(lock);
	}
	return (old);
}
