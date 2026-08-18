/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/18 11:39:41 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	main(int argc, char *argv[])
{
	t_data	config;

	if (argc != 9)
	{
		printf(
			"Error: The 'len' argument is missing, "
			"or there are too many of the 9 required arguments!\n");
		return (1);
	}
	if (!pars(&config, (char **)argv))
	{
		printf("Error: Regarding the argument!");
		return (1);
	}
	printf("Number of coders %d\n", config.number_of_coders);
	printf("Time to burnout %d\n", config.time_to_burnout);
	printf("Time to compile %d\n", config.time_to_compile);
	printf("Time to debug: %d\n", config.time_to_debug);
	printf("Time to refactor: %d\n", config.time_to_refactor);
	printf(
		"Number of compiles_required: %d\n", config.number_of_compiles_required
		);
	printf("Dongle_cooldown: %d\n", config.dongle_cooldown);
	printf("Scheduler: %s\n", config.scheduler);
	return (0);
}
