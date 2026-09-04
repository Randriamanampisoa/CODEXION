/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 11:14:21 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/04 13:41:04 by fanilran         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_dongle(t_coder *coder, t_dongle *dongle)
{
	long	timestamp;

	pthread_mutex_lock(&dongle->lock);
	while (dongle->available != 1)
		pthread_cond_wait(&dongle->cond, &dongle->lock);
	dongle->available = 0;
	pthread_mutex_unlock(&dongle->lock);
	pthread_mutex_lock(&coder->config->print_mutex);
	timestamp = get_timestamp_ms(coder->config->start_time);
	printf("%ld %d has taken a dongle\n",timestamp, coder->id);
	pthread_mutex_unlock(&coder->config->print_mutex);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->lock);
	dongle->available = 1;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->lock);
}
