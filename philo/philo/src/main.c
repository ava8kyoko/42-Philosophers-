/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:24:51 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/17 19:55:03 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static bool	detect_starvation(t_table *t, int i_death, int i_meal, bool finish)
{
	while (1)
	{
		pthread_mutex_lock(&t->p[i_death].time);
		if (get_time(0, 0) - t->p[i_death].time_last_meal > t->time_to_die)
		{
			pthread_mutex_lock(&t->print);
			printf("%lu %d %s\n", get_time(true, t->p[i_death].time_start),
				i_death + 1, "died");
			return (DEAD);
		}
		pthread_mutex_unlock(&t->p[i_death].time);
		i_death = (i_death + 1) % t->nbr_of_philo;
		if (t->nbr_of_meal != -1)
		{
			finish = false;
			pthread_mutex_lock(&t->p[i_meal].meal);
			if (t->p[i_meal].meal_to_eat == 0)
				finish = true;
			pthread_mutex_unlock(&t->p[i_meal].meal);
			if (finish)
				i_meal--;
			if (i_meal + 1 == 0)
				return (SATIATED);
		}
	}
}

static void	philosophers_process(t_table *t)
{
	int		end;
	int		id;

	t->starting_time = get_time(0, 0);
	id = -1;
	while (++id < t->nbr_of_philo)
	{
		t->p[id].time_start = t->starting_time;
		t->p[id].time_last_meal = t->starting_time;
		pthread_create(&t->p[id].thread, NULL, &philosophers_routine,
			&t->p[id]);
		pthread_detach(t->p[id].thread);
	}
	end = detect_starvation(t, 0, t->nbr_of_philo - 1, false);
	id = -1;
	while (++id < t->nbr_of_philo)
	{
		pthread_mutex_lock(&t->p[id].end_main_to_philo);
		t->p[id].ending = end;
		pthread_mutex_unlock(&t->p[id].end_main_to_philo);
	}
	return ;
}

static bool	init_philo(t_table *t)
{
	int	i;

	i = -1;
	while (++i < t->nbr_of_philo)
	{
		if (pthread_mutex_init(&t->p[i].end_main_to_philo, NULL)
			|| pthread_mutex_init(&t->p[i].fork_left, NULL)
			|| pthread_mutex_init(&t->p[i].meal, NULL)
			|| pthread_mutex_init(&t->p[i].time, NULL))
			return (false);
		t->p[i].fork_right = &t->p[(i + 1) % t->nbr_of_philo].fork_left;
		t->p[i].state = START;
		t->p[i].philo_id = i + 1;
		t->p[i].time_to_die = t->time_to_die;
		t->p[i].time_to_eat = t->time_to_eat;
		t->p[i].time_to_sleep = t->time_to_sleep;
		t->p[i].meal_to_eat = t->nbr_of_meal;
		t->p[i].time_start = 0;
		t->p[i].time_last_meal = 0;
		t->p[i].t = t;
		t->p[i].ending = false;
	}
	return (true);
}

static bool	init_table(t_table *t, int argc, char **argv)
{
	if (pthread_mutex_init(&t->print, NULL))
		return (false);
	t->nbr_of_philo = ft_atoi(argv[1]);
	t->time_to_die = ft_atoi(argv[2]);
	t->time_to_eat = ft_atoi(argv[3]);
	t->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		t->nbr_of_meal = ft_atoi(argv[5]);
	else
		t->nbr_of_meal = -1;
	if (t->nbr_of_philo < 1 || t->nbr_of_philo > 200 
		|| t->time_to_die < 10 || t->time_to_eat <= 0
		|| t->time_to_sleep <= 0 || (argc == 6 && t->nbr_of_meal <= 0))
		return (false);
	t->starting_time = 0;
	return (true);
}

int	main(int argc, char **argv)
{
	static t_table	t;
	int				i;

	if (argc < 5 || argc > 6)
	{
		ft_putstr_fd("Error: Invalid argc\n", 2);
		return (EXIT_FAILURE);
	}
	i = 0;
	while (argv[++i])
	{
		if (is_valid_int(argv[i]) == false)
		{
			ft_putstr_fd("Error: Invalid argv\n", 2);
			return (EXIT_FAILURE);
		}
	}
	if (init_table(&t, argc, argv) == false || init_philo(&t) == false)
	{
		ft_putstr_fd("Error: Initialisation failure\n", 2);
		return (EXIT_FAILURE);
	}
	philosophers_process(&t);
	return (EXIT_SUCCESS);
}
