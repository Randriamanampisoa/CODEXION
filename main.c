/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/21 15:07:06 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *arg)
{
	t_data	*config;
	int		i;

	config = (t_data *)arg;
	i = 0;
	while (i < config->number_of_compiles_required)
	{
		printf("Compile\n");
		usleep(config->time_to_compile * 1000);
		i++;
	}
	return (NULL);
}

void	*create_threads(t_data config)
{
	pthread_t *thread;

	int	i;
	i = 0;
	thread = malloc(sizeof(pthread_t) * config.number_of_coder);
	while (config.number_of_coder > i)
	{
		if (pthread_create(&thread[i], NULL, routine, (void *)&config) != 0)
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
	t_coder		*tab;
	int			i;

	if (!pars(&config, argc, (char **)argv))
	{
		printf("Error: Regarding the argument!");
		return (1);
	}
	tab = NULL;
	i = 0;
	tab = malloc(sizeof(t_coder) * config.number_of_coder);
	if (!tab)
		return (1);
	while (i < config.number_of_coder)
	{
		tab[i].id = i + 1;
		i++;
	}
	create_threads(config);
	free(tab);
	return (0);
}
