/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/26 16:09:31 by fanilran         ###   ########.fr       */
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
		coders_take_d(coder);
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
	create_threads(config, coders);
	i = 0;
	while (i < config.number_of_coder)
	{
		pthread_mutex_destroy(&dongles[i].lock);
		pthread_cond_destroy(&dongles[i].cond);
		i++;
	}
	pthread_mutex_destroy(&config.print_mutex);
	free(dongles);
	free(coders);
	return (0);
}
