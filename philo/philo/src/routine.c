/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchampag <mchampag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/25 10:25:11 by mchampag          #+#    #+#             */
/*   Updated: 2023/08/17 20:03:49 by mchampag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo.h"

static bool	need_to_sleep(t_philo *p)
{
	long int	time_to_stop;

	time_to_stop = 0;
	if (p->state == EAT)
	{
		pthread_mutex_lock(&p->time);
		p->time_last_meal = get_time(0, 0);
		time_to_stop = p->time_last_meal + p->time_to_eat;
		pthread_mutex_unlock(&p->time);
		if (print_state(p, "is eating"))
			return (true);
		p->state = SLEEP;
	}
	else if (p->state == SLEEP)
	{
		if (print_state(p, "is sleeping"))
			return (true);
		pthread_mutex_lock(&p->t->print);
		time_to_stop = p->time_last_meal + p->time_to_eat + p->time_to_sleep;
		pthread_mutex_unlock(&p->t->print);
		p->state = THINK;
	}
	return (make_it_sleep(p, time_to_stop));
}

static bool	is_eating(t_philo *p)
{
	if (need_to_sleep(p))
		return (true);
	pthread_mutex_unlock(&p->fork_left);
	pthread_mutex_unlock(p->fork_right);
	return (false);
}

static bool	is_taking_forks(t_philo *p)
{
	while (p->state != EAT)
	{
		if (p->state == START || p->state == FORK_RIGHT)
		{
			pthread_mutex_lock(&p->fork_left);
			if (print_state(p, "has taken a fork"))
				return (true);
			if (p->state == FORK_RIGHT)
				p->state = EAT;
			else if (p->state == START)
				p->state = FORK_LEFT;
		}
		else if (p->state == FORK_LEFT)
		{
			pthread_mutex_lock(p->fork_right);
			if (print_state(p, "has taken a fork"))
				return (true);
			if (p->state == FORK_LEFT)
				p->state = EAT;
			else if (p->state == THINK)
				p->state = FORK_RIGHT;
		}
	}
	return (false);
}

void	*philosophers_routine(void *arg)
{
	t_philo	*p;

	p = arg;
	if (p->philo_id % 2 == 0)
		usleep(1200);
	while (1)
	{
		if (p->state == START && is_taking_forks(p))
			break ;
		if (p->state == EAT && is_eating(p))
			break ;
		if (p->state == SLEEP && need_to_sleep(p))
			break ;
		if (p->state == THINK)
		{
			if (print_state(p, "is thinking"))
				break ;
			p->state = START;
		}
	}
	return ((void *)0);
}
