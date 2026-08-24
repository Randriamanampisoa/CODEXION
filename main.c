/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/24 16:33:53 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *arg)
{
	t_coder	*coder;
	int		i;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->config->number_of_compiles_required)
	{
		printf("Compile\n");
		usleep(coder->config->time_to_compile * 1000);
		i++;
	}
	return (NULL);
}

void	*create_threads(t_data config, t_coder *coders)
{
	pthread_t *thread;
	int	i;

	thread = malloc(sizeof(pthread_t) * config.number_of_coder);
	if (!thread)
		return (NULL);
	i = 0;
	while (config.number_of_coder > i)
	{
		if (pthread_create(&thread[i], NULL, routine, &coders[i]) != 0)
			return (NULL);
		i++;
	}
	i = 0;
	while (config.number_of_coder > i)
	{
		pthread_join(thread[i], NULL);
		i++;
	}

	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data		config;
	t_coder		*coders;
	t_dongle	*dongles;
	int			i;

	if (!pars(&config, argc, (char **)argv))
	{
		printf("Error: Regarding the argument!");
		return (1);
	}
	dongles = malloc(sizeof(t_dongle) * config.number_of_coder);
	if (!dongles)
		return (1);
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_mutex_init(&dongles[i].lock, NULL);
		pthread_cond_init(&dongles[i].cond, NULL);
		dongles[i].available = 1;
		dongles[i].released_at = 0;
		i++;
	}
	coders = malloc(sizeof(t_coder) * config.number_of_coder);
	if (!coders)
	{
		free(dongles);
		return (1);
	}
	i = 0;
	while (i < config.number_of_coder)
	{
		coders[i].id = i + 1;
		coders[i].config = &config;
		coders[i].left = &dongles[i];
		coders[i].right = &dongles[(i + 1) % config.number_of_coder];
		i++;
	}
	create_threads(config, coders);
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	free(dongles);
	free(coders);
	return (0);
}
