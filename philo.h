/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/05 16:07:33 by oeddamou          #+#    #+#             */
/*   Updated: 2025/04/20 19:57:24 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_philo
{
	pthread_t		thread;
	int				meals_eaten;
	long			last_meal;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	meal_lock;
}					t_philo;

typedef struct s_data
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				n_eat;
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	id_lock;
	pthread_mutex_t	finish_lock;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	int				id;
	int				finish;
}					t_data;

void				ft_print(t_data *d, int id, char type);
int					ft_dead(t_data *d);
int					ft_get(int *value, pthread_mutex_t *lock);
int					ft_set(int *value, pthread_mutex_t *lock, int new_val,
						char type);
int					ft_atoi(char *str);
long				ft_time(void);
int					ft_usleep(long milliseconds, t_data *d);
int					ft_stock_arg(int ac, char **av, t_data *d);
int					ft_eat(t_data *d, int id);
void				*ft_routine(void *d);
int					ft_inisial(t_data *d);
void				ft_easy_init(int ac, char **av, t_data *d);

#endif