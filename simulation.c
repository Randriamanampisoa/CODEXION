/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 11:14:21 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/11 12:33:18 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	take_dongle(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		take_one(coder, coder->left);
		take_one(coder, coder->right);
	}
	else
	{
		take_one(coder, coder->right);
		take_one(coder, coder->left);
	}
	return (1);
}

int	check_burnout(t_coder *coders)
{
	int	i;
	long	d;
	long	n;

	i = 0;
	while (i < coders->config->number_of_coder)
	{
		pthread_mutex_lock(&coders[i].activity_mutex);
		d = coders[i].last_compile_start + coders[i].config->time_to_burnout;
		pthread_mutex_unlock(&coders[i].activity_mutex);
		n = get_timestamp_ms(coders[i].config->start_time);
		if (n >= d)
		{
			printf("%ld %d is bunrout\n", n, coders[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->config->number_of_compiles_required)
	{
		take_dongle(coder);
		pthread_mutex_lock(&coder->activity_mutex);
		coder->last_compile_start = get_timestamp_ms(coder->config->start_time);
		pthread_mutex_unlock(&coder->activity_mutex);
		compiles(coder);
		release_dongle(coder, coder->left);
		release_dongle(coder, coder->right);
		debuges(coder);
		refactores(coder);
		i++;
		pthread_mutex_lock(&coder->activity_mutex);
		coder->compile_done = i;
		pthread_mutex_unlock(&coder->activity_mutex);
	}
	return (NULL);
}

void	*monitor(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		if (check_burnout(coder))
			break ;
		usleep(1000);
	}
	return (NULL);	
}
