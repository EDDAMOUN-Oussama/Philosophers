/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 16:49:23 by oeddamou          #+#    #+#             */
/*   Updated: 2025/04/26 20:37:30 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <signal.h>
# include <sys/time.h>

// typedef struct s_data
// {
// 	int			num_of_philos;
// 	int			time_to_die;
// 	int			time_to_eat;
// 	int			time_to_sleep;
// 	long		start_time;
// 	int			n_eat;
//     sem_t       *forks_sem;
//     sem_t       *print_sem;
// }				t_data;

typedef struct s_philo
{
	// t_data		data;
	int			id;
	int			n_eat;
	int			meals_eaten;
	int			num_of_philos;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	long		last_meal;
	long		start_time;
	pid_t 		*pid;
    sem_t       *print_sem;
    sem_t       *forks_sem;
    sem_t       *last_meal_sem;
	pthread_t	thread;
}				t_philo;

int				ft_atoi(char *str);
long			ft_time(void);
int				ft_usleep(long milliseconds);

#endif