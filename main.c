/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/19 17:47:46 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*routine(void *arg)
{
	t_data	*config;
	int		i;

	config = (t_data *)arg;
	i = 0;
	while (i < config->number_of_coder)
	{
		usleep(100000);
		printf("Time to burnout %d\n", config->time_to_burnout);
		printf("Time to compile %d\n", config->time_to_compile);
		printf("Time to debug: %d\n", config->time_to_debug);
		printf("Time to refactor: %d\n", config->time_to_refactor);
		printf(
			"Number of compiles_required: %d\n",
			config->number_of_compiles_required
			);
		printf("Dongle_cooldown: %d\n", config->dongle_cooldown);
		printf("Scheduler: %s\n", config->scheduler);
		i++;
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_data		config;
	pthread_t	thread1;
	pthread_t	thread2;
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
	pthread_create(&thread1, NULL, routine, (void *)&config);
	pthread_create(&thread2, NULL, routine, (void *)&config);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	free(tab);
	return (0);
}
