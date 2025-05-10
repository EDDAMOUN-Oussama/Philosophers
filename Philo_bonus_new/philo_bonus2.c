/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oeddamou <oeddamou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:29:08 by oeddamou          #+#    #+#             */
/*   Updated: 2025/05/10 10:47:17 by oeddamou         ###   ########.fr       */
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
	d->forks_sem = sem_open("/forks", O_EXCL | O_CREAT, 0644, d->num_of_philos);
	d->print_sem = sem_open("/print", O_EXCL | O_CREAT, 0644, 1);
	if (!d->forks_sem || !d->print_sem)
    {
        sem_unlink("/forks");
	    sem_unlink("/print");
        printf("Error: open semaphore fatal\n");
        exit(1);
    }
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

int    ft_create_sem(t_philo *p, char *name)
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
        return (0);
    return (1);
}

void    ft_inisial(t_data *d, t_philo *p)
{
    int i;
    int s;

    i = -1;
    while (++i < d->num_of_philos)
    {
		memset(&p[i], 0, sizeof(t_philo));
        p[i].data = d;
		p[i].id = i + 1;
		p[i].meals_eaten = 0;
        if (!ft_create_sem(&p[i], "/meals_eat"))
        {
            printf("Error: open semaphore fatal\n");
            // while (--i >= 0)
            // {
            //     sem_close(p[i].last_meal_sem);
            //     sem_unlink(p[i].name_sem);
            // }
		    // sem_close(d->forks_sem);
		    // sem_close(d->print_sem);
            // sem_unlink("/forks");
	        // sem_unlink("/print");
            // free(p);
            // exit(1);
            ft_clean(p, 1, i, 0);
        }
    }
    i = -1;
    d->start_time = ft_time();
    while (++i < d->num_of_philos)
    {
        p[i].pid = fork();
        if (p[i].pid < 0)
        {
            // while (--i >= 0)
			// 	kill(p[i--].pid, 9);
            // while (++i < d->num_of_philos)
			// 	wait(NULL);
            ft_kill(p, i);
			printf("Error in fork\n");
            // while (--i >= 0)
            // {
            //     sem_close(p[i].last_meal_sem);
            //     sem_unlink(p[i].name_sem);
            // }
            // sem_close(d->forks_sem);
	        // sem_close(d->print_sem);
	        // sem_unlink("/forks");
	        // sem_unlink("/print");
            // free(p);
            // exit(1);
            ft_clean(p, 1, d->num_of_philos, 0);
        }
        if (p[i].pid == 0)
            ft_routine(&p[i]);
    }
    i = -1;
    while (++i < d->num_of_philos)
    {
        waitpid(-1, &s, 0);
        if (s != 0)
        {
            // i = -1;
            // while (++i < d->num_of_philos)
            //     kill(p[i].pid, 9);
			// i = -1;
			// while (++i < d->num_of_philos)
			// 	wait(NULL);
            ft_kill(p, d->num_of_philos);
		}
    }
}

int main(int ac, char **av)
{
    t_data data;
    t_philo *p;
    
    if (ac > 6 || ac < 5 || !ft_data_init(ac, av, &data))
	{
		write(2, "Error: argements not valid\n", 27);
		return (1);
	}
    if (data.n_eat == 0)
        return (0);
    p = malloc(sizeof(t_philo) * data.num_of_philos);
    if (!p)
    {
        printf("Error: malloc falid\n");
        exit(1);
    }
    ft_inisial(&data, p);
    ft_clean(p, 1, data.num_of_philos, 0);
    return (0);
}
