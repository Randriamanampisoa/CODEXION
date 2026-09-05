/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/05 14:22:28 by fanilran         ###   ########.fr       */
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


void	*routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->config->number_of_compiles_required)
	{
		take_dongle(coder);
		// pthread_mutex_lock(&coder->activity_mutex);
		// coder->last_compile_start = get_timestamp_ms(coder->config->start_time);
		// pthread_mutex_unlock(&coder->activity_mutex);
		compiles(coder);
		release_dongle(coder->left);
		release_dongle(coder->right);
		debuges(coder);
		refactores(coder);
		i++;
	}
	return (NULL);
}

void	*create_threads(t_data config, t_coder *coders)
{
	pthread_t	*thread;
	int			i;

	thread = malloc(sizeof(pthread_t) * config.number_of_coder);
	if (!thread)
		return (NULL);
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_create(&thread[i], NULL, routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	free(thread);
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
	config.start_time = get_timestamp_ms(0);
	create_threads(config, coders);
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
