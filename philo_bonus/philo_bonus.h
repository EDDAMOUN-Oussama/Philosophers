/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:29:20 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/18 15:59:23 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <semaphore.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data
{
	int			num_of_philos;
	int			t_to_die;
	int			t_to_eat;
	int			t_to_sleep;
	int			n_eat;
	sem_t		*print_sem;
	sem_t		*forks_sem;
	sem_t		*finish_sem;
	time_t		start_time;
	pthread_t	thread;
}				t_data;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	char		name_sem[14];
	pid_t		pid;
	sem_t		*last_meal_sem;
	t_data		*data;
	time_t		last_meal;
	pthread_t	thread;
}				t_philo;

int				ft_atoi(char *str);
long			ft_time(void);
int				ft_usleep(long milliseconds, t_philo *p);
long			get_last_meal(t_philo *p);
void			ft_clean(t_philo *p, int ext, int nb, int type);
void			ft_kill(t_philo *p, int stop);
void			ft_print(t_philo *p, char type);
void			ft_routine(t_philo *p);
void			ft_wait_kill(t_philo *p, t_data *d);
void			*ft_wait_finish(void *data);
void			ft_close(t_philo *p);

#endif