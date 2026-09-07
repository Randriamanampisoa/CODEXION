/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fanilran <fanilran@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/16 04:30:25 by fanilran          #+#    #+#             */
/*   Updated: 2026/09/07 16:53:37 by fanilran         ###   ########.fr       */
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
	int		stopped;

	coder = (t_coder *)arg;
	i = 0;
	while (i < coder->config->number_of_compiles_required)
	{
		pthread_mutex_lock(&coder->config->stop_mutex);
		stopped = coder->config->stop;
		pthread_mutex_unlock(&coder->config->stop_mutex);
		if (stopped)
			break ;
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

int	check_burnout(t_data *config, t_coder *coder)
{
	int	i;
	long	now;
	long	deadline;

	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_mutex_lock(&coder[i].activity_mutex);
		deadline = coder[i].last_compile_start + config->time_to_burnout;
		pthread_mutex_unlock(&coder[i].activity_mutex);
		now = get_timestamp_ms(config->start_time);
		if (now > deadline)
		{
			printf("%d %ld burned out", coder[i].id, deadline);
			return (0);
		}
		i++;
	}
	return (1);
}

int	check_all_done(t_data *config, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_mutex_lock(&coder[i].activity_mutex);
		if (coder[i].compile_done < config->number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder[i].activity_mutex);
			return (1);
		}
		pthread_mutex_unlock(&coder[i].activity_mutex);
		i++;
	}
	return (0);
}

void	*monitor(void *arg)
{
	t_monitor	*m;

	m = (t_monitor *)arg;
	while (1)
	{
		pthread_mutex_lock(&m->config->stop_mutex);
		if (m->config->stop)
		{
			pthread_mutex_unlock(&m->config->stop_mutex);
			break ;
		}
		pthread_mutex_unlock(&m->config->stop_mutex);
		if (check_burnout(m->config, m->coders) || check_all_done(m->config, m->coders))
		{
			pthread_mutex_lock(&m->config->stop_mutex);
			m->config->stop = 1;
			pthread_mutex_unlock(&m->config->stop_mutex);
			break ;
		}
		usleep(1000);
	}
	return (NULL);
}

void	*create_threads(t_data *config, t_coder *coders)
{
	pthread_t	*thread;
	pthread_t	monitor_thread;
	t_monitor	m;
	int			i;

	m.config = config;
	m.coders = coders;
	pthread_create(&monitor_thread, NULL, monitor, &m);
	thread = malloc(sizeof(pthread_t) * config->number_of_coder);
	if (!thread)
		return (NULL);
	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_create(&thread[i], NULL, routine, &coders[i]);
		i++;
	}
	i = 0;
	while (i < config->number_of_coder)
	{
		pthread_join(thread[i], NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
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
	config.stop = 0;
	config.start_time = get_current_ms();
	pthread_mutex_init(&config.stop_mutex, NULL);
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
