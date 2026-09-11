/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/11 12:04:18 by fanilran         ###   ########.fr       */
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

void	*create_threads(t_data *config, t_coder *coders)
{
	int			i;
	pthread_t	monitor_t;

	coders->thread = malloc(sizeof(pthread_t) * config->number_of_coder);
	if (!coders->thread)
		return (NULL);
	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_create(&coders->thread[i], NULL, routine, &coders[i]);
		i++;
	}
	pthread_create(&monitor_t, NULL, monitor, coders);
	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_join(coders->thread[i], NULL);
		i++;
	}
	pthread_join(monitor_t, NULL);
	free(coders->thread);
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data		config;
	t_coder		*coders;
	t_dongle	*dongles;
	int			i;

	if (!pars(&config, argc, (char **)argv))
		return (1);
	if (!init_coder_dongle(&config, &coders, &dongles))
		return (1);
	config.start_time = get_current_ms();
	//printf("Start time: %ld\n\n", get_current_ms());
	create_threads(&config, coders);
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		pthread_mutex_destroy(&coders[i].activity_mutex);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&config.print_mutex);
	free(dongles);
	free(coders);
	return (0);
}
