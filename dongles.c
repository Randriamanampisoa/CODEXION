/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 11:14:21 by fanilran          #+#    #+#             */
/*   Updated: 2026/08/26 15:12:28 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);

	while (!dongle->available)
		pthread_cond_wait(&dongle->cond, &dongle->lock);

	dongle->available = 0;

	pthread_mutex_unlock(&dongle->lock);

	pthread_mutex_lock(&coder->config->print_mutex);
	printf("%d has taken a dongle\n", coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
}

void	release_dongle(t_coder *coder, t_dongle *dongle)
{
	(void)coder;
	pthread_mutex_lock(&dongle->lock);

	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}

void	coders_take_d(t_coder *coder)
{
	if (coder->id % 2 == 0)
	{
		take_dongle(coder, coder->left);
		take_dongle(coder, coder->right);
	}
	else
	{
		take_dongle(coder, coder->right);
		take_dongle(coder, coder->left);
	}
	compiles(coder);

	release_dongle(coder, coder->left);
	release_dongle(coder, coder->right);

	debuges(coder);
	refactores(coder);
}