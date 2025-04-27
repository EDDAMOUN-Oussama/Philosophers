/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 11:36:11 by oeddamou          #+#    #+#             */
/*   Updated: 2025/04/27 08:46:48 by oeddamou         ###   ########.fr       */
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

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

int	ft_usleep(long milliseconds)
{
	long	start;

	start = ft_time();
	while ((ft_time() - start) < milliseconds)
	{
		usleep(100);
	}
	return (0);
}
