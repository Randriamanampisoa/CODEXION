/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 15:01:17 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/27 11:13:30 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compiles(t_coder *coder)
{
	pthread_mutex_lock(&coder->config->print_mutex);
	printf("%d is compiling\n", coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
	usleep(coder->config->time_to_compile * 1000);
}

void	debuges(t_coder *coder)
{
	pthread_mutex_lock(&coder->config->print_mutex);
	printf("%d is debugging\n", coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
	usleep(coder->config->time_to_debug * 1000);
}

void	refactores(t_coder *coder)
{
	pthread_mutex_lock(&coder->config->print_mutex);
	printf("%d is refactoring\n", coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
	usleep(coder->config->time_to_refactor * 1000);
}
