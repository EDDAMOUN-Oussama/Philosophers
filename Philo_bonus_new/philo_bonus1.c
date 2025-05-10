/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:29:08 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/05 16:46:54 by oeddamou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_data_init(int ac, char **av, t_data *d)
{
	memset(d, 0, sizeof(t_data));
	d->num_of_philos = ft_atoi(av[1]);
	d->t_to_die = ft_atoi(av[2]);
	d->t_to_eat = ft_atoi(av[3]);
	d->t_to_sleep = ft_atoi(av[4]);
	d->n_eat = -2;
	if (ac == 6)
		d->n_eat = ft_atoi(av[5]);
	if (d->num_of_philos < 1 || d->t_to_die < 1 || d->t_to_eat < 1
		|| d->t_to_sleep < 1 || d->n_eat == -1)
		return (0);
	sem_unlink("/forks");
	sem_unlink("/print");
	sem_unlink("/start_sem");
	d->forks_sem = sem_open("/forks", O_EXCL | O_CREAT, 0644, d->num_of_philos);
	d->print_sem = sem_open("/print", O_EXCL | O_CREAT, 0644, 1);
	d->start_sem = sem_open("/start_sem", O_EXCL | O_CREAT, 0644, d->num_of_philos);
	if (!d->forks_sem || !d->print_sem || !d->start_sem)
        exit (1);
    d->start_time = ft_time();
    return (1);
}

void	ft_print(t_philo *p, char type)
{
	long	date;

	date = ft_time() - p->data->start_time;
	sem_wait(p->data->print_sem);
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

long	get_last_meal(t_philo *p)
{
	long last_meal;
	
	sem_wait(p->last_meal_sem);
	last_meal = p->last_meal;
	sem_post(p->last_meal_sem);
	return (last_meal);
}

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
			// sem_close(p->data->forks_sem);
			// sem_close(p->data->print_sem);
			// printf ("test form %d\n", p->id);
			// sem_close(p->last_meal_sem);
			// sem_close(p->data->start_sem);
	        sem_unlink(p->name_sem);
			exit(-1);
		}
		usleep(900);
	}
	return (NULL);
}

void    ft_routine(t_philo *p)
{
    sem_wait(p->data->start_sem);
    // p->data->start_time = ft_time();
	// p->last_meal = p->data->start_time;
	p->last_meal = ft_time();
    if (p->data->num_of_philos == 1)
    {
		ft_print(p, 'f');
        ft_usleep(p->data->t_to_die + 1, p);
		printf("%ld %d died\n", ft_time() - p->data->start_time, p->id);
    	sem_wait(p->data->print_sem);
		sem_close(p->data->forks_sem);
		sem_close(p->data->print_sem);
		sem_close(p->last_meal_sem);
		sem_close(p->data->start_sem);
	    sem_unlink(p->name_sem);
		exit(-1);
    }
	if (p->id % 2 == 0)
		usleep(p->data->t_to_eat / 2 * 1000);
	if (!pthread_create(&p->thread, NULL, ft_test_dead, (void *)p))
    while (1)
    {
        sem_wait(p->data->forks_sem);
		ft_print(p, 'f');
		sem_wait(p->data->forks_sem);
		ft_print(p, 'f');
		ft_print(p, 'e');
    	ft_usleep(p->data->t_to_eat, p);
        sem_post(p->data->forks_sem);
        sem_post(p->data->forks_sem);
        sem_wait(p->last_meal_sem);
        p->last_meal = ft_time();
        sem_post(p->last_meal_sem);
		p->meals_eaten++;
        if (p->meals_eaten >= p->data->n_eat && p->data->n_eat >= 0)
        {
            sem_close(p->last_meal_sem);
            sem_close(p->data->forks_sem);
	        sem_close(p->data->print_sem);
            sem_close(p->data->start_sem);
	        sem_unlink(p->name_sem);
            exit(0);
        }
        ft_print(p, 's');
		ft_usleep(p->data->t_to_sleep, p);
		ft_print(p, 't');
    }
}

void    ft_create_sem(t_philo *p, char *name)
{
    int i;

    i = 0;
    while (name[i])
    {
        p->name_sem[i] = name[i];
        i++;
    }
    p->name_sem[i++] = ((int)(p->id / 100) + '0');
    p->name_sem[i++] = ((int)((p->id % 100) / 10) + '0');
    p->name_sem[i++] = ((int)((p->id % 100) % 10) + '0');
    p->name_sem[i++] = '\0';
	sem_unlink(p->name_sem);
	p->last_meal_sem = sem_open(p->name_sem, O_EXCL | O_CREAT, 0644, 1);
    if (!p->last_meal_sem)
        exit(-1);
}

void    ft_inisial(t_data *d)
{
    t_philo *p;
    int i;
    int s;

    if (d->n_eat == 0)
        return ;
    p = malloc(sizeof(t_philo) * d->num_of_philos);
    if (!p)
        exit(1);
    i = -1;
    while (++i < d->num_of_philos)
    {
		memset(&p[i], 0, sizeof(t_philo));
        p[i].data = d;
		p[i].id = i + 1;
		p[i].meals_eaten = 0;
        ft_create_sem(&p[i], "/meals_eat");  
        sem_wait(d->start_sem);
    }
    i = -1;
    while (++i < d->num_of_philos)
    {
        p[i].pid = fork();
        if (p[i].pid < 0)
        {
            while (--i >= 0)
				kill(p[i--].pid, 9);
			printf("error in fork\n");
            free(p);
            sem_close(d->forks_sem);
	        sem_close(d->print_sem);
        	sem_close(d->start_sem);
	        sem_unlink("/forks");
	        sem_unlink("/print");
	        sem_unlink("/start_sem");
            exit(1);
        }
		usleep (1000);
        if (p[i].pid == 0)
            ft_routine(&p[i]);
    }
	usleep(100000);
    i = -1;
    while (++i < d->num_of_philos)
        sem_post(d->start_sem);
    i = -1;
    while (++i < d->num_of_philos)
    {
        waitpid(-1, &s, 0);
        if (s != 0)
        {
            i = -1;
            while (++i < d->num_of_philos)
                kill(p[i].pid, 9);
			i = -1;
			while (++i < d->num_of_philos)
				wait(NULL);
		}
    }
}

int main(int ac, char **av)
{
    t_data data;
    
    if (ac > 6 || ac < 5 || !ft_data_init(ac, av, &data))
	{
		write(2, "Error: argements not valid\n", 27);
		return (1);
	}
    ft_inisial(&data);
	
}